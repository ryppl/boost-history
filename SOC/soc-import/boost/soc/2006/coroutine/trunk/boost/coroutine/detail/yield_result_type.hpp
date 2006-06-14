//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_YIELD_RESULT_TYPE_HPP_20060609
#define BOOST_COROUTINE_DETAIL_YIELD_RESULT_TYPE_HPP_20060609
#include <boost/coroutine/detail/make_tuple.hpp>
#include <boost/mpl/vector.hpp>
namespace boost{ namespace coroutines { namespace detail {
    /**
     * Same as make_tuple, but if
     * @p TypeList is singular, @p type is the single element, and
     * if TypeList is empty, @p type is @p void.
     */
    template<typename TypeList>
    struct yield_result_type {
      typedef typename make_tuple<TypeList>::type type;
    };
    
    template<typename T>
    struct yield_result_type <boost::mpl::vector1<T> >{
      typedef T type;
    };
    
    template<>
    struct yield_result_type <boost::mpl::vector0<> >{
      typedef void type;
    };
} } }
#endif
