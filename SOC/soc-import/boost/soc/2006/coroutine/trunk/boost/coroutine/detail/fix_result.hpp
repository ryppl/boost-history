//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_FIX_RESULT_HPP_20060709
#define BOOST_COROUTINE_DETAIL_FIX_RESULT_HPP_20060709
#include <boost/utility/enable_if.hpp>
namespace boost { namespace coroutines { namespace detail {
    template<typename Traits>
    inline
    void
    fix_result(const typename Traits::as_tuple&, 
	       typename 
	       boost::enable_if_c<Traits::length == 0>::type * = 0){}


    template<typename Traits>
    inline
    typename Traits::template at<0>::type
    fix_result(const typename Traits::as_tuple& x, 
	       typename 
	       boost::enable_if_c<Traits::length == 1>::type * = 0){
      using boost::get;
      return get<0>(x);
    }


    template<typename Traits>
    inline
    typename Traits::as_tuple
    fix_result(const typename Traits::as_tuple& x, 
	       typename 
	       boost::enable_if_c<(Traits::length > 1)>::type* =0){
      return x;
    }
} } }
#endif
