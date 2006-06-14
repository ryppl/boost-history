//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_COROUTINE_TRAITS_HPP_20060613
#define BOOST_COROUTINE_DETAIL_COROUTINE_TRAITS_HPP_20060613
#include <boost/function_traits.hpp>
#include <boost/coroutine/detail/yield_result_type.hpp>


namespace boost { namespace coroutines { namespace detail {
  template<typename Signature>
  struct coroutine_traits {    
    typedef typename boost::function_traits<Signature>::result_type 
    result_type;

    typedef typename detail::signature<Signature>::type arg_vector;

    typedef typename detail::yield_result_type<
      typename detail::signature<Signature>::type>::type
    yield_result_type;

    typedef /**/ yield_arg_vector;

    typedef typename detail::default_type<
      typename boost::function_traits<
      Signature>::result_type>::type
    result_slot_type;

    typedef  typename detail::make_tuple
    <typename detail::signature<Signature>::type >::type 
    arg_slot_type;

    typedef  /**/
    arg_slot_traits;

  };
} } }
#endif
