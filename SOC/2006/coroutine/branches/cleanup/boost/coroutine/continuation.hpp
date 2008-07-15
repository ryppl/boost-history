// Copyright (c) 2006, Giovanni P. Deretta
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_CONTINUATION_20080321
# define BOOST_COROUTINE_CONTINUATION_20080321
# include "detail/context_interface.hpp"
# include <boost/type_traits/is_same.hpp>
# include <stdlib.h>
namespace boost{ namespace coroutines{ 

  namespace detail {
    extern "C" void trampoline(void *) { }

    struct no_argument {};
  }

  /// Typeless one shot continuation. 
  ///
  /// Represents the reified state of execution of one computation as
  /// captured at a specific point. The saved state can then be resumed
  /// later. Once captured, a continuation must be fired exactly once. 
  ///
  /// There are two ways to capture a continuation: 
  ///
  /// - <tt>make_continuation(f)</tt> will return a new continuation
  /// that when fired will invoke @p f.  
  ///
  /// - firing another continuation will capture the current
  /// continuation and it will be passed by the fired continuation.
  ///
  /// @tparam Context is the system specific context
  /// implementation. Models ContextInterface
  /// 
  /// @invariant 
  ///
  ///  - a continuation object always refer to an invokable state
  /// unless <tt>fired() == false</tt>. 
  ///
  /// - once fired a continuation will remain in the fired state
  /// unless another continuation is move-assigned to it.
  ///
  /// @note @c continuation is copyable, but all copies will refer to the
  /// same logical continuaiton. Once a copy is fired, all other
  /// copies are invalidated.
  template<typename Context, typename PType = detail::no_argument>
  class continuation_tpl {
    typedef Context context_interface;
    typedef PType param_type;

    template<typename Functor>
    struct bound {
      static void callcc1_target(void * x) {
        bound& b = *static_cast<bound*>(x);

        // We do not know how long b is going to be valid so we need
        // to copy stuff out of it: first retrive the just created context;
        continuation_tpl continuation = b.continuation;

        // is that what we wanted?
        BOOST_ASSERT(continuation.fired());
        
        // result of user function call.
        continuation_tpl final;
        {
          // get a local copy of the user function ...
          Functor functor = b.functor;
          
          // ... and of the caller continuation ...
          continuation_tpl caller = *continuation.m_chain;

          BOOST_ASSERT(!caller.fired());

          // ... pass it to the user function... which will eventually
          // return the continuation we will have to exit to.

          final = functor(caller);        
          
          // Functor is destroyed here
        }
        
        BOOST_ASSERT(!final.fired());
        
        // ok, now call the exit continuation.  we can't use
        // operator() because it will create a new continuation and we
        // do not want it to. This buffer is just to make swap_context
        // happy, we won't be reusing it.
        context_storage dummy;
        
        // chain does not point to the caller continuation in this case, but
        // to the continuation to be deleted...
        BOOST_ASSERT(continuation.fired());
        final.m_chain->m_chain = &continuation;

        // ... thus mark it as such.
        continuation.m_state = state::delete_this;
      
        context_interface::swap_context(dummy, final.m_ctx, hints::unspecified());
      
        // we will never reach this point, so 'continuation' and
        // 'final' destructors won't ever be be called.  It doesn't
        // matter as their destructors have no side effects.
        BOOST_ASSERT("the impossible happened: a dead continuation has been revived");
      }
      
      void * in_guard;
      Functor functor;
      continuation_tpl continuation;
      void * out_guard;
    };
  public:

#ifdef MOVE
    /// Move constructor.
    ///
    /// Construct a continuation from an rvalue continuation. 
    ///
    /// @post <tt>this->fired() == original_from.fired() && from.fired() == true  </tt>.
    continuation_tpl(move_from<continuation_tpl> from) {
      BOOST_ASSERT(&*from != this);
      m_chain = from->m_chain;
      m_ctx = move(from->m_ctx);
      m_state = from->m_state;
      from->m_state = state::fired;
    }
#endif

    continuation_tpl(const continuation_tpl & from) {
      BOOST_ASSERT(&from != this);
      m_chain = from.m_chain;
      m_ctx = from.m_ctx;
      m_state = from.m_state;
      //from.m_state = state::fired;
    }
    
    /// Move assignement operator.
    ///
    /// Assign an rvalue right hand side to @c this.
    ///
    /// @post <tt>this->fired() == original_rhs.fired() && rhs.fired() == true  </tt>.
#ifdef MOVE
    continuation_tpl&
    operator=(move_from<continuation_tpl> rhs) {
      BOOST_ASSERT(&*rhs != this);
      m_chain = rhs->m_chain;
      m_ctx = rhs->m_ctx;
      m_state = rhs->m_state;
      return *this;
    }
#endif

    continuation_tpl&
    operator=(const continuation_tpl & rhs) {
      BOOST_ASSERT(&rhs != this);
      m_chain = rhs.m_chain;
      m_ctx = rhs.m_ctx;
      m_state = rhs.m_state;
      //rhs.m_state = state::fired;
      return *this;
    }

    /// Invoke this continuation. 
    ///
    /// First the current state is saved in a new continuation, then
    /// this continuation state is restored. The saved state can be
    /// retrived by the restored continuation. This function returns
    /// when the saved state is restored again.
    ///
    /// @pre <tt>fired() == false</tt>
    ///
    /// @post <tt>fired() == true</tt>
    ///
    /// @return an initialized continuation if 
    continuation_tpl operator()() {
      BOOST_ASSERT(!fired());
      m_state = state::fired;
      continuation_tpl * otherp = m_chain;
      BOOST_ASSERT(otherp); // m_chain points to the original 'current'
                           // (i.e. the continuation 'this' is a copy
                           // of) at the other side of swap_context.
      BOOST_ASSERT(otherp->fired());
      
      continuation_tpl current;
      current.m_state = state::ready;
      current.m_chain = &current; // the new continuation points to
                                  // itself.

      otherp->m_chain = &current; // send a pointer to the new
                                 // continuation to the other side.

      // invoke this context and save into current...
      context_interface::swap_context(current.m_ctx, m_ctx, hints::unspecified());
      // ... and now we are back!

      BOOST_ASSERT(current.fired());

      continuation_tpl other = *current.m_chain;
      if (other.m_state == state::delete_this) {
        context_interface::destroy_context(other.m_ctx);
        other.m_state = state::fired;

      } else {
        BOOST_ASSERT(!other.fired());
      }
      return other;
    }

    template<typename F>
    friend 
    continuation_tpl 
    callcc1(F f, param_type in, param_type& out, std::ptrdiff_t stack_size = context_interface::default_stack_size) {
      bound<F> b = { (void*)0xDEADBEEF, f, continuation_tpl(), (void*)0xDEADBEEF };

      // make a new continuation that will invoke f.
      detail::callback_type cb = &bound<F>::callcc1_target;
      context_interface::make_context
        ( b.continuation.m_ctx, 
          cb, 
          (void*)&b,
          (std::ptrdiff_t)stack_size);

      b.continuation.m_state = state::ready;
      b.continuation.m_chain = &b.continuation;

      // then call it.
      return b.continuation();

    }


    template<typename F>
    friend 
    continuation_tpl 
    callcc1(F f,  std::ptrdiff_t stack_size = context_interface::default_stack_size) {
      static const int i = boost::is_same<param_type, void>::value;
      BOOST_STATIC_ASSERT((i));
      bound<F> b = { (void*)0xDEADBEEF, f, continuation_tpl(), (void*)0xDEADBEEF };

      // make a new continuation that will invoke f.
      detail::callback_type cb = &bound<F>::callcc1_target;
      context_interface::make_context
        ( b.continuation.m_ctx, 
          cb, 
          (void*)&b,
          (std::ptrdiff_t)stack_size);

      b.continuation.m_state = state::ready;
      b.continuation.m_chain = &b.continuation;

      // then call it.
      return b.continuation();

    }

    /// Destructor.
    /// @pre <tt>fired() == true</tt>
    ~continuation_tpl() {
      //BOOST_ASSERT(fired());
    }

    /// Check if the continuation has been invoked.
    bool fired() const { return m_state == state::fired; }

  protected:

    /// Return the continuation of the caller of this continuation.
    /// @pre this continuation must be active.
    /// <tt>fired() == true</tt>.
    continuation_tpl& caller_continuation() const {
      BOOST_ASSERT(fired());
      // the caller is a valid continuation or is a dead continuation.
      BOOST_ASSERT(!m_chain->fired()|| m_chain->m_chain == m_chain);
      return *m_chain;
    }

    typedef BOOST_DEDUCED_TYPENAME context_interface::context_storage context_storage;;

    struct state {
      enum my_state { dangling, ready, fired, delete_this };
    };

    continuation_tpl * m_chain;
    context_storage m_ctx;
    BOOST_DEDUCED_TYPENAME state::my_state m_state;
    param_type m_p;

    continuation_tpl() 
      : m_chain()
      , m_ctx()
      , m_state(state::dangling) {}

  private:

  };
}}
#endif
