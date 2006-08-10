//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_COROUTINE_IMPL_HPP_20060601
#define BOOST_COROUTINE_COROUTINE_IMPL_HPP_20060601
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable: 4355) //this used in base member initializer
#endif 
#include <cstddef>
#include <algorithm> //for swap
#include <boost/intrusive_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/detail/atomic_count.hpp>
#include <boost/coroutine/exception.hpp>
#include <boost/coroutine/detail/argument_unpacker.hpp>
#include <boost/coroutine/detail/swap_context.hpp>
#include <boost/coroutine/detail/coroutine_accessor.hpp>

/*
 * Currently asio can in some cases, call copy constructors and
 * operator= from different threads, even if in the
 * one-thread-per-service model. 
 * This will be corrected in future versions, but for now
 * we will play it safe and use an atomic count.
 */
#define BOOST_COROUTINE_ATOMIC_COUNT

namespace boost { namespace coroutines { namespace detail {
	
  const std::ptrdiff_t default_stack_size = -1;

  template<typename ContextImpl>
  class context_base : public ContextImpl {
  public:

    typedef ContextImpl context_impl;
    typedef context_base<context_impl> type;
    typedef boost::intrusive_ptr<type> context_ptr;
    typedef void deleter_type(const type*);
    
    template<typename Derived>
	context_base(Derived& derived, std::ptrdiff_t stack_size) :
      context_impl(derived, stack_size),
      m_counter(0),
      m_deleter(&deleter<Derived>),
      m_state(ctx_ready), 
      m_exit_state(ctx_exit_not_requested),
      m_exit_status(ctx_not_exited),
      m_wait_counter(0),
      m_type_info(0) {}
    
    friend
    void intrusive_ptr_add_ref(type * ctx) {
      ctx->acquire();
    }
    
    friend
    void intrusive_ptr_release(type * ctx) {
      ctx->release();
    }
      
    bool unique() const {
      return count() == 1;
    }

    std::size_t count() const {
      return m_counter;
    }
      
    void acquire() const {
      ++m_counter;
    }
      
    void release() const {
      BOOST_ASSERT(m_counter);
      if(--m_counter == 0) {
	m_deleter(this);
      }
    }

    /**
     * A signal may occur only when a context is 
     * not running (is delivered sinchrononously).
     * This means that state MUST NOT be busy.
     * It may be ready or waiting.
     * returns 'ready()'
     */
    bool signal () {
      BOOST_ASSERT(!running() && !exited());
      if(m_wait_counter) --m_wait_counter;
      if(!m_wait_counter && m_state == ctx_waiting)
	m_state = ctx_ready;      
      return ready();
    }

    /**
     * Wake up a waiting context.
     */
    void wake_up() {
    }
    /*
     * Returns true if the context is runnable.
     */
    bool ready() const {
      return m_state == ctx_ready;
    }

    /*
     * Returns true if the context is in wait
     * state.
     */
    bool waiting() const {
      return m_state == ctx_waiting;
    }

    bool running() const {
      return m_state == ctx_running;
    }

    bool exited() const {
      return m_state == ctx_exited;
    }

    void invoke() {
      BOOST_ASSERT(ready());
      do_invoke();
      if(m_exit_status) {
	if(m_exit_status == ctx_exited_abnormally) {
	std::type_info const * tinfo =0;
	std::swap(m_type_info, tinfo);
	throw abnormal_exit(tinfo?*tinfo: typeid(unknown_exception_tag));
	} else if(m_exit_status == ctx_exited_exit)
	  throw coroutine_exited();
      }
    }

    void yield() {
      BOOST_ASSERT(m_exit_state < ctx_exit_signaled); //prevent infinite loops
      BOOST_ASSERT(running());

      m_state = ctx_ready;
      do_yield();

      BOOST_ASSERT(m_state == ctx_running);
      check_exit_state();
    }

    /*
     * In n > 0, put the coroutine in the wait state
     * then return to caller. If n = 0 do nothing.
     * The coroutine will remain in the wait state it
     * is signaled 'n' times.
     */
    void wait(int n) {
      BOOST_ASSERT(m_exit_state < ctx_exit_signaled); //prevent infinite loops
      BOOST_ASSERT(running());

      if(n == 0) return;
      m_wait_counter = n;

      m_state = ctx_waiting;
      do_yield();

      BOOST_ASSERT(m_state == ctx_running);
      check_exit_state();
      BOOST_ASSERT(m_wait_counter == 0);
    }

    void yield_to(context_base& to) {
      BOOST_ASSERT(m_exit_state < ctx_exit_signaled); //prevent infinite loops
      BOOST_ASSERT(m_state == ctx_running);

      if(to.exited())  throw coroutine_exited();
      if(!to.ready()) throw coroutine_not_ready();

      std::swap(m_caller, to.m_caller);
      std::swap(m_state, to.m_state);
      swap_context(*this, to, detail::yield_to_hint());

      BOOST_ASSERT(m_state == ctx_running);
      check_exit_state();
    }

    void exit() {
      if(m_exit_state < ctx_exit_pending) 
	m_exit_state = ctx_exit_pending;	
      if(ready() || waiting()) {
	do_invoke();
	BOOST_ASSERT(exited()); //at this point the coroutine MUST have exited.
      } else {
	check_exit_state();
      }
    }

    ~context_base() {
      BOOST_ASSERT(!running());
      try {
	if(!exited())
	  exit();
      } catch(...) {}
    }

  protected:
    // global coroutine state
    enum context_state {
      ctx_running,  // context running.
      ctx_ready,    // context at yield point.
      ctx_waiting,     // context waiting for events.
      ctx_exited    // context is finished.
    };

    // exit request state
    enum context_exit_state {
      ctx_exit_not_requested,  // exit not requested.
      ctx_exit_pending,   // exit requested.
      ctx_exit_signaled,  // exit request delivered.
    };
    
    // exit status
    enum context_exit_status {
      ctx_not_exited,
      ctx_exited_return,  // process exited by return.
      ctx_exited_exit,    // process exited by exit().
      ctx_exited_abnormally // process exited uncleanly.
    };

    void check_exit_state() {
      if(!m_exit_state) return;
      if(m_state == ctx_running)
	  throw exit_exception();
    }

    void do_return(context_exit_status status, std::type_info const* info) {
      BOOST_ASSERT(status != ctx_not_exited);
      BOOST_ASSERT(m_state == ctx_running);
      m_type_info = info;
      m_state = ctx_exited;
      m_exit_status = status;
      do_yield();
    }

  private:

    void do_yield() {
      swap_context(*this, m_caller, detail::yield_hint());
    }

    void do_invoke() {
      BOOST_ASSERT(ready() || waiting());
      m_state = ctx_running;
      swap_context(m_caller, *this, detail::invoke_hint());
    }

    template<typename ActualCtx>
    static void deleter (const type* ctx){
      delete static_cast<ActualCtx*>(const_cast<type*>(ctx));
    }
            
    typedef typename context_impl::context_impl_base ctx_type;
    ctx_type m_caller;
    mutable 
#ifndef BOOST_COROUTINE_ATOMIC_COUNT
    std::size_t
#else
    boost::detail::atomic_count
#endif
    m_counter;
    deleter_type * m_deleter;
    context_state m_state;
    context_exit_state m_exit_state;
    context_exit_status m_exit_status;
    unsigned int m_wait_counter;    
    std::type_info const* m_type_info;
  };

  template<typename CoroutineType, typename ContextImpl>
  class coroutine_impl:
    public context_base<ContextImpl>
  {
  public:
    template<typename DerivedType, typename ResultType>
    friend class add_result;

    typedef ContextImpl context_impl;
    typedef CoroutineType coroutine_type;
    typedef coroutine_impl<coroutine_type, context_impl> type;
    typedef context_base<context_impl> context_base;
    typedef typename coroutine_type::arg_slot_type arg_slot_type;
    typedef typename coroutine_type::result_slot_type result_slot_type;
    typedef boost::intrusive_ptr<type> pointer;
  
    template<typename DerivedType>
	coroutine_impl(DerivedType * this_, std::ptrdiff_t stack_size) :
      context_base(*this_, stack_size),
	m_arg(0),
	m_result(0){}
                
    arg_slot_type * args() {
      BOOST_ASSERT(m_arg);
      return m_arg;
    };
    
    result_slot_type * result() {
      BOOST_ASSERT(m_result);
      return this->m_result;
    } 

    template<typename Functor>
	static inline	
	pointer create(Functor, std::ptrdiff_t);

    void bind_args(arg_slot_type* arg) {
      m_arg = arg;
    }
    
    void bind_result(result_slot_type* res) {
      m_result = res;
    }
  private:
    arg_slot_type * m_arg;
    result_slot_type * m_result;
  };

  template<typename FunctorType, typename CoroutineType, typename ContextImpl>
  class coroutine_impl_wrapper :
    public coroutine_impl<CoroutineType, ContextImpl> {
  public:
    typedef coroutine_impl_wrapper<FunctorType, CoroutineType, ContextImpl> 
    type;
    typedef CoroutineType coroutine_type;
    typedef typename CoroutineType::result_type result_type;
    typedef coroutine_impl<CoroutineType, ContextImpl> super_type;

    typedef FunctorType functor_type;
	coroutine_impl_wrapper(functor_type f, std::ptrdiff_t stack_size) :

		super_type(this, stack_size),
      m_fun(f){}

    void operator()() {
      typedef typename super_type::context_exit_status
	context_exit_status;
      context_exit_status status = super_type::ctx_exited_return;
      std::type_info const* tinfo = 0;
      try {
	this->check_exit_state();
	do_call<result_type>();
      } catch (const exit_exception&) {
	status = super_type::ctx_exited_exit;
      } catch (std::exception const& e) {
	status = super_type::ctx_exited_abnormally;
	tinfo = &typeid(e);
      } catch (...) {
	status = super_type::ctx_exited_abnormally;
      }
      this->do_return(status, tinfo);	  
    }
  public:    

    //GCC workaround as per enable_if docs 
    template <int> struct dummy { dummy(int) {} };
    /**
     * Implementation for operator()
     * @note: This is tricky, cancel_count is used to prevent 'this'
     * to increase the reference count of itself. 
     * This is used only for the first call.    
     */
    template<typename ResultType>
    typename boost::enable_if<boost::is_void<ResultType> >::type
    do_call(dummy<0> = 0) {
      BOOST_ASSERT(this->count() > 0);
      typedef BOOST_DEDUCED_TYPENAME
	coroutine_type::self self_type;
      boost::optional<self_type> self (coroutine_accessor::in_place(this));
      detail::unpack_ex
	/*<typename coroutine_type::arg_slot_traits>*/
	(m_fun, *self, *this->args(), (typename coroutine_type::arg_slot_traits*)0);
    }

    template<typename ResultType>
    typename boost::disable_if<boost::is_void<ResultType> >::type
    do_call(dummy<1> = 1) {
      BOOST_ASSERT(this->count() > 0);
      typedef BOOST_DEDUCED_TYPENAME
      coroutine_type::self self_type;
      boost::optional<self_type> self (coroutine_accessor::in_place(this));
      typedef BOOST_DEDUCED_TYPENAME coroutine_type::arg_slot_traits traits;
	  
      //note: placement new.
      new(this->result()) BOOST_DEDUCED_TYPENAME coroutine_type::result_slot_type
	(detail::unpack_ex
	 /*<traits>*/
	 (m_fun, *self, *this->args(), (traits*)0));
    }
  
    FunctorType m_fun;
  };

  template<typename CoroutineType, typename ContextImpl>
  template<typename Functor>
  inline
  typename 
  coroutine_impl<CoroutineType, ContextImpl>::pointer
  coroutine_impl<CoroutineType, ContextImpl>::
  create(Functor f, std::ptrdiff_t stack_size = default_stack_size) {
    return new coroutine_impl_wrapper<Functor, CoroutineType, ContextImpl>
      (f, stack_size);      
  }

} } }
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif
