//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_COROUTINE_TRAITS_HPP_20060613
#define BOOST_COROUTINE_DETAIL_COROUTINE_TRAITS_HPP_20060613
#include <boost/type_traits.hpp>
#include <boost/coroutine/tuple_traits.hpp>
#include <boost/coroutine/detail/yield_result_type.hpp>

namespace boost { namespace coroutines { namespace detail {

  template<typename T>
  struct as_tuple {
    typedef typename T::as_tuple type;
  };

  // This trait class is used to compute
  // all nested typedefs of coroutines given
  // a signature in the form 'result_type(parm1, ... parmn)'.
  template<typename Signature>
  struct coroutine_traits {    
  private:
    typedef BOOST_DEDUCED_TYPENAME boost::function_traits<Signature>::result_type 
    signature_result_type;
  public:
    typedef BOOST_DEDUCED_TYPENAME
    boost::mpl::eval_if<is_tuple_traits<signature_result_type>,
			as_tuple<signature_result_type>,
			boost::mpl::identity<signature_result_type> >
    ::type result_type;

    typedef BOOST_DEDUCED_TYPENAME
    boost::mpl::eval_if<is_tuple_traits<signature_result_type>,
			boost::mpl::identity<signature_result_type>,
			boost::mpl::if_
			<boost::is_same<signature_result_type, void>,
			 tuple_traits<>,
			 tuple_traits<signature_result_type> > >
    ::type result_slot_traits;
    
    typedef BOOST_DEDUCED_TYPENAME result_slot_traits
    ::as_tuple result_slot_type;
    	     
    typedef BOOST_DEDUCED_TYPENAME detail::make_tuple_traits
    <typename detail::signature<Signature>::type >
    ::type arg_slot_traits;

    typedef BOOST_DEDUCED_TYPENAME arg_slot_traits
    ::as_tuple arg_slot_type;

    typedef BOOST_DEDUCED_TYPENAME arg_slot_traits
    ::as_result yield_result_type;

  };
} } }
#endif
