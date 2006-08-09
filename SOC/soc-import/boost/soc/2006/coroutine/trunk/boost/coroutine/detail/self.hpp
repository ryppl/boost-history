//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_SELF_HPP_20060709
#define BOOST_COROUTINE_DETAIL_SELF_HPP_20060709
#include <boost/noncopyable.hpp>
#include <boost/coroutine/detail/fix_result.hpp>
#include <boost/coroutine/detail/coroutine_accessor.hpp>
#include <boost/coroutine/detail/signal.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace coroutines { namespace detail {

#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400))
#define BOOST_COROUTINE_VCPP80_WORKAROUND 
#else
//for now, define this unconditionally for testing.
#define BOOST_COROUTINE_VCPP80_WORKAROUND 
#endif

  template<typename Coroutine>
  class coroutine_self :boost::noncopyable {
  public:
    typedef Coroutine coroutine_type;
    typedef coroutine_self<coroutine_type> type;
    friend class detail::coroutine_accessor;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::impl_type impl_type;

    // Note, no reference counting here.
    typedef impl_type * impl_ptr;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::result_type result_type;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::result_slot_type result_slot_type;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::yield_result_type yield_result_type;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::result_slot_traits result_slot_traits;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::arg_slot_type arg_slot_type;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::arg_slot_traits arg_slot_traits;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type
    ::yield_traits yield_traits;
#ifndef BOOST_COROUTINE_VCPP80_WORKAROUND 
#   define BOOST_COROUTINE_param_with_default(z, n, type_prefix)    \
    BOOST_DEDUCED_TYPENAME call_traits                              \
    <BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)>::param_type \
    BOOST_PP_CAT(arg, n) =                                          \
    BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)()             \
/**/

    yield_result_type yield
    (BOOST_PP_ENUM
       (BOOST_COROUTINE_ARG_MAX,
	BOOST_COROUTINE_param_with_default,
	BOOST_DEDUCED_TYPENAME yield_traits::arg)) {
      return yield_impl
	(BOOST_DEDUCED_TYPENAME 
	 coroutine_type::result_slot_type(BOOST_PP_ENUM_PARAMS
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
     * 'typedef typename call_traits<typename coroutine_type::yield_traits::argN_type>::param_type yield_call_argN_type;'
     */
    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, 
		    BOOST_COROUTINE_param_typedef, 
		    (BOOST_DEDUCED_TYPENAME 
		     coroutine_type::yield_traits::arg, yield_call_arg));

    /*
     * Generate lines like this:
     * 'typedef typename call_traits<typename coroutine_type::argN_type>::param_type call_argN_type;'
     */
    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		    BOOST_COROUTINE_param_typedef,
		    (BOOST_DEDUCED_TYPENAME 
		     coroutine_type::arg, call_arg));

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
		    BOOST_DEDUCED_TYPENAME 
		    coroutine_type::yield_traits::arg))) {
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
      (BOOST_DEDUCED_TYPENAME Target::self::call_arg, 
       BOOST_DEDUCED_TYPENAME Target::arg))) {
      typedef typename Target::arg_slot_type type;
      return yield_to_impl
	(target, type
	 (BOOST_PP_ENUM_PARAMS
	  (BOOST_COROUTINE_ARG_MAX, arg)));
    }
#endif

#undef  BOOST_COROUTINE_param_with_default

  private:
    coroutine_self(impl_type * pimpl, detail::init_from_impl_tag) :
      m_pimpl(pimpl) {}

    yield_result_type yield_impl(const BOOST_DEDUCED_TYPENAME 
				 coroutine_type::result_slot_type& result) {
      BOOST_ASSERT(m_pimpl);
      this->m_pimpl->result() = result;
      this->m_pimpl->yield();    
      return detail::fix_result<
	BOOST_DEDUCED_TYPENAME
	coroutine_type::arg_slot_traits>(m_pimpl->args());
    }

    template<typename TargetCoroutine>
    yield_result_type yield_to_impl(TargetCoroutine& target, 
			   BOOST_DEDUCED_TYPENAME TargetCoroutine
			   ::arg_slot_type args) {
      BOOST_ASSERT(m_pimpl);

      coroutine_accessor::get_impl(target)->bind_args(&args);
      coroutine_accessor::get_impl(target)->bind_result(&m_pimpl->result());    
      this->m_pimpl->yield_to(*coroutine_accessor::get_impl(target));
      return detail::fix_result<
	BOOST_DEDUCED_TYPENAME
	coroutine_type::arg_slot_traits>(m_pimpl->args());
    }

    impl_ptr get_impl() {
      return m_pimpl;
    }
    impl_ptr m_pimpl;
  };
} } }

#endif
