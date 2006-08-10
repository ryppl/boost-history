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
#include <boost/optional.hpp>
#include <boost/coroutine/detail/argument_unpacker.hpp>
#include <boost/coroutine/detail/coroutine_accessor.hpp>
#include <boost/coroutine/detail/context_base.hpp>

namespace boost { namespace coroutines { namespace detail {
	
  // This class augment the contest_base class with
  // the coroutine signature type.
  // This is mostly just a place to put
  // typesafe argument and result type pointers.
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
    typedef typename coroutine_type::result_type result_type;
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
      BOOST_ASSERT(*m_result);
      return *this->m_result;
    } 

    template<typename Functor>
	static inline	
	pointer create(Functor, std::ptrdiff_t);

    void bind_args(arg_slot_type* arg) {
      m_arg = arg;
    }
    
    void bind_result(result_slot_type* res) {
      *m_result = res;
    }

    // Another level of indirecition is needed to handle
    // yield_to correctly.
    void bind_result_pointer(result_slot_type** resp) {
      m_result = resp;
    }

    result_slot_type** result_pointer() {
      return m_result;
    }
  protected:
    boost::optional<result_slot_type>  m_result_last;

  private:
    arg_slot_type * m_arg;
    result_slot_type ** m_result;

  };

  // This type augment coroutine_impl type with the type of the stored 
  // functor. The type of this object is erased right after construction
  // when it is assigned to a pointer to coroutine_impl. A deleter is
  // passed down to make it sure that the correct derived type is deleted.
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
    /*
     * Implementation for operator()
     * This is for void result types.
     * Can throw if m_fun throws. At least it can throw exit_exception.
     */
    template<typename ResultType>
    typename boost::enable_if<boost::is_void<ResultType> >::type
    do_call(dummy<0> = 0) {
      BOOST_ASSERT(this->count() > 0);
      typedef BOOST_DEDUCED_TYPENAME
	coroutine_type::self self_type;
      boost::optional<self_type> self (coroutine_accessor::in_place(this));
      detail::unpack_ex
	(m_fun, 
	 *self, 
	 *this->args(), 
	 detail::trait_tag<typename coroutine_type::arg_slot_traits>());

      typedef BOOST_DEDUCED_TYPENAME coroutine_type::result_slot_type 
	result_slot_type;

      // In this particulare case result_slot_type is guaranteed to be
      // default constructible.
      this->m_result_last = result_slot_type();
      this->bind_result(&*this->m_result_last);
    }

    // Same as above, but for non void result types.
    template<typename ResultType>
    typename boost::disable_if<boost::is_void<ResultType> >::type
    do_call(dummy<1> = 1) {
      BOOST_ASSERT(this->count() > 0);
      typedef BOOST_DEDUCED_TYPENAME
      coroutine_type::self self_type;

      boost::optional<self_type> self (coroutine_accessor::in_place(this));

      typedef BOOST_DEDUCED_TYPENAME coroutine_type::arg_slot_traits traits;
      typedef BOOST_DEDUCED_TYPENAME coroutine_type::result_slot_type 
	result_slot_type;
	  
      this->m_result_last = boost::in_place(result_slot_type(detail::unpack_ex
			   (m_fun, 
			    *self, 
			    *this->args(), 
			    detail::trait_tag<traits>())));      
      
      this->bind_result(&*this->m_result_last);
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
