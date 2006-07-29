//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_COROUTINE_HPP_20060512
#define BOOST_COROUTINE_COROUTINE_HPP_20060512
#include <cstddef>
#include <boost/preprocessor/repetition.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/detail/coroutine_impl.hpp>
#include <boost/coroutine/detail/default_context_impl.hpp>
#include <boost/coroutine/detail/is_callable.hpp>
#include <boost/coroutine/detail/argument_packer.hpp>
#include <boost/coroutine/detail/argument_unpacker.hpp>
#include <boost/coroutine/detail/signature.hpp>
#include <boost/coroutine/detail/index.hpp>
#include <boost/coroutine/detail/signal.hpp>
#include <boost/coroutine/detail/coroutine_traits.hpp>

#ifdef _MSC_VER
#define BOOST_COROUTINE_VCPP80_WORKAROUND 
#else

//for now, define this unconditionally for testing.
#define BOOST_COROUTINE_VCPP80_WORKAROUND 
#endif
namespace boost { namespace coroutines {



  template<typename Signature, 
	   typename ContextImpl = detail::default_context_impl>
  class coroutine  {
  public:
    typedef coroutine<Signature, ContextImpl> type;
    typedef ContextImpl context_impl;
    typedef Signature signature_type;
    friend class detail::coroutine_accessor;
    friend class detail::wait_gateway;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::result_type result_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::yield_result_type  yield_result_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::result_slot_type result_slot_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::result_slot_traits result_slot_traits;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::arg_slot_type arg_slot_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::arg_slot_traits arg_slot_traits;
        
    typedef detail::coroutine_impl<type, context_impl> impl_type;
    typedef BOOST_DEDUCED_TYPENAME  impl_type::pointer impl_ptr;  
   
    coroutine(): m_pimpl(0) {}

    template<typename Functor>
    explicit 
    coroutine (Functor f, 
		   std::ptrdiff_t stack_size = detail::default_stack_size,
	       BOOST_DEDUCED_TYPENAME boost::enable_if<detail::is_callable<Functor> >
	       ::type * = 0
	       ) :
      m_pimpl(impl_type::create(f, stack_size)) {}
    
    coroutine(const coroutine& rhs) :
      m_pimpl(rhs.m_pimpl) {}

    coroutine& operator=(coroutine rhs) {
      return swap(rhs);
    }

    template<typename Functor>
    coroutine& operator= (Functor rhs) {
      coroutine(rhs).swap(*this);
      return *this;
    }

    coroutine& swap(coroutine& rhs) {
      std::swap(m_pimpl, rhs.m_pimpl);
      return *this;
    }

    friend
    inline 
    void swap(coroutine& lhs, coroutine& rhs) {
      lhs.swap(rhs);
    }

#   define BOOST_COROUTINE_generate_argument_n_type(z, n, traits_type) \
    typedef BOOST_DEDUCED_TYPENAME traits_type ::template at<n>::type  \
    BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);                         \
    /**/

    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		    BOOST_COROUTINE_generate_argument_n_type,
		    arg_slot_traits);

    static const int arity = arg_slot_traits::length;

    struct yield_traits {
      BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		      BOOST_COROUTINE_generate_argument_n_type,
		      result_slot_traits);
      static const int arity = result_slot_traits::length;
    };

#   undef BOOST_COROUTINE_generate_argument_n_type
   
#   define BOOST_COROUTINE_param_with_default(z, n, type_prefix)    \
    BOOST_DEDUCED_TYPENAME call_traits                              \
    <BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)>::param_type \
    BOOST_PP_CAT(arg, n) =                                          \
    BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)()             \
/**/

    result_type operator()
      (BOOST_PP_ENUM
       (BOOST_COROUTINE_ARG_MAX,
	BOOST_COROUTINE_param_with_default,
	arg)) {
      return call_impl
	(arg_slot_type(BOOST_PP_ENUM_PARAMS
	  (BOOST_COROUTINE_ARG_MAX, 
	   arg)));
      }

#ifndef BOOST_COROUTINE_VCPP80_WORKAROUND 
    yield_result_type yield
    (BOOST_PP_ENUM
       (BOOST_COROUTINE_ARG_MAX,
	BOOST_COROUTINE_param_with_default,
	BOOST_DEDUCED_TYPENAME yield_traits::arg)) {
      return yield_impl
	(result_slot_type(BOOST_PP_ENUM_PARAMS
			  (BOOST_COROUTINE_ARG_MAX, 
			   arg)));
    }
    
    template<typename Target>
    yield_result_type yield_to
    (Target& target
     BOOST_PP_ENUM_TRAILING
     (BOOST_COROUTINE_ARG_MAX,
      BOOST_COROUTINE_param_with_default,
      typename Target::arg)) {
      typedef BOOST_DEDUCED_TYPENAME Target::arg_slot_type type;
      return yield_to_impl
	(target, type(BOOST_PP_ENUM_PARAMS
	  (BOOST_COROUTINE_ARG_MAX, 
	   arg)));
      }
#else
    
    
  private:
    /* 
     * VC8.0 can't handle the call_traits meta-invocation inside
     * a function parameter list (except when it does, see operator()). 
     * Splitting it in separate typedefs
     * fixes the problem.
     */
#define BOOST_COROUTINE_param_typedef(z, n, prefix_tuple)  \
    typedef BOOST_DEDUCED_TYPENAME                   \
    call_traits<                                     \
    BOOST_PP_CAT                                     \
    (BOOST_PP_CAT                                    \
     (BOOST_PP_TUPLE_ELEM(2, 0, prefix_tuple), n),   \
     _type)                                          \
      >::param_type                                  \
    BOOST_PP_CAT                                     \
    (BOOST_PP_CAT                                    \
     (BOOST_PP_TUPLE_ELEM(2, 1, prefix_tuple), n),   \
     _type)                                          \
     /**/;
    
    /*
     * Generate lines like this:
     * 'typedef typename call_traits<typename yield_traits::argN_type>::param_type yield_call_argN_type;'
     */
    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, 
		    BOOST_COROUTINE_param_typedef, 
		    (BOOST_DEDUCED_TYPENAME yield_traits::arg, yield_call_arg));

    /*
     * Generate lines like this:
     * 'typedef typename call_traits<argN_type>::param_type call_argN_type;'
     */
    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		    BOOST_COROUTINE_param_typedef,
		    (arg, call_arg));

#undef BOOST_COROUTINE_param_typedef
  public:
#undef  BOOST_COROUTINE_param_with_default
#define BOOST_COROUTINE_param_with_default(z, n, prefix_tuple) \
    BOOST_PP_CAT(BOOST_PP_CAT                                  \
		 (BOOST_PP_TUPLE_ELEM(2, 0, prefix_tuple),     \
		  n), _type)                                   \
      BOOST_PP_CAT(arg, n) =                                   \
    BOOST_PP_CAT(BOOST_PP_CAT                                  \
		 (BOOST_PP_TUPLE_ELEM(2, 1, prefix_tuple),     \
		  n), _type)()                                 \
      /**/
     
    yield_result_type yield
    (BOOST_PP_ENUM(BOOST_COROUTINE_ARG_MAX,
		   BOOST_COROUTINE_param_with_default,
		   (yield_call_arg ,
		    BOOST_DEDUCED_TYPENAME yield_traits::arg))) {
      return yield_impl
      (result_slot_type
       (BOOST_PP_ENUM_PARAMS(BOOST_COROUTINE_ARG_MAX,arg)));
    }

    template<typename Target>
    yield_result_type yield_to
    (Target& target
     BOOST_PP_ENUM_TRAILING
     (BOOST_COROUTINE_ARG_MAX,
      BOOST_COROUTINE_param_with_default,
      (BOOST_DEDUCED_TYPENAME Target::call_arg, 
       BOOST_DEDUCED_TYPENAME Target::arg))) {
      typedef typename Target::arg_slot_type type;
      return yield_to_impl
	(target, type
	 (BOOST_PP_ENUM_PARAMS
	  (BOOST_COROUTINE_ARG_MAX, arg)));
    }
#endif

#undef  BOOST_COROUTINE_param_with_default

    typedef void(coroutine::*bool_type)();
    operator bool_type() const {
      return good()? &coroutine::bool_type_f: 0;
    }

    bool operator==(const coroutine& rhs) {
      return m_pimpl == rhs.m_pimpl;
    }

    void exit() {
      BOOST_ASSERT(m_pimpl);
      m_pimpl->exit();
    }
  private:

    bool signal() {
      return m_pimpl->signal();
    }

    void wait(int n) {
      m_pimpl->wait(n);
    }

    /**
     * The second parameter is used to avoid calling this constructor
     * by mistake from other member funcitons (specifically operator=).
     */
    coroutine(impl_type * pimpl, detail::init_from_impl_tag) :
      m_pimpl(pimpl) {}

    void bool_type_f() {}

    bool good() const  {
      return m_pimpl && !m_pimpl->exited();
    }

    template<typename Traits>
    static inline
    void
    fix_result(const typename Traits::as_tuple&, 
	       typename 
	       boost::enable_if_c<Traits::length == 0>::type * = 0){}


    template<typename Traits>
    static inline
    typename Traits::template at<0>::type
    fix_result(const typename Traits::as_tuple& x, 
	       typename 
	       boost::enable_if_c<Traits::length == 1>::type * = 0){
      using boost::get;
      return get<0>(x);
    }


    template<typename Traits>
    static inline
    typename Traits::as_tuple
    fix_result(const typename Traits::as_tuple& x, 
	       typename 
	       boost::enable_if_c<(Traits::length > 1)>::type* =0){
      return x;
    }

    /**
     * @note The use of fix_result is very likely to 
     * prevent NRVO. This could be worked around, but for now 
     * leave it as is. For constructors and destructors without
     * side effects the compiler still does a good job.
     * @note The current code requires result type(s) to be 
     * DefaultConstructible. This can be avoided using
     * boost::optional, but it would further prevent NRVO.
     * This can't be proabably avoided without using dirty tricks
     * (i.e. somehow getting the address of the return frame).
     * ... later... nonsense just do not bother with NRVO. With simple types
     * the compiler already does a good job. With complex types
     * it probably does not matter.
     */
    result_type call_impl(arg_slot_type args) {
      BOOST_ASSERT(m_pimpl);

      result_slot_type result;
      m_pimpl->bind_args(&args);
      m_pimpl->bind_result(&result);
      m_pimpl->invoke();
      return fix_result<result_slot_traits>(result);
    }

    yield_result_type yield_impl(const result_slot_type& result) {
      BOOST_ASSERT(m_pimpl);
      m_pimpl->result() = result;
      m_pimpl->yield();    
      return fix_result<arg_slot_traits>(m_pimpl->args());
    }

    template<typename TargetSignature>
    yield_result_type yield_to_impl(coroutine<TargetSignature>& target, 
			   typename coroutine<TargetSignature>
			   ::arg_slot_type args) {
      BOOST_ASSERT(m_pimpl);

      target.m_pimpl->bind_args(&args);
      target.m_pimpl->bind_result(&m_pimpl->result());    
      m_pimpl->yield_to(*target.m_pimpl.get());
      return fix_result<arg_slot_traits>(m_pimpl->args());
    }

    impl_ptr m_pimpl;
  };
} }
#endif
