///////////////////////////////////////////////////////////////////////////////
// arithmetic::equal.hpp                                                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ARITHMETIC_EQUAL_HPP_ER_2009
#define BOOST_ARITHMETIC_EQUAL_HPP_ER_2009
#include <cmath>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/math/tools/precision.hpp>

namespace boost{
namespace arithmetic_tools{ //arithmetic already exists as class or namespace

    template<typename T>
    typename boost::enable_if<
        typename is_float<T>::type,
        bool
    >::type
    equal(const T& a,const T& b){
        static T eps = boost::math::tools::epsilon<T>();
        return fabs(a-b)<eps;
    }

    template<typename T>
    typename enable_if<
        typename is_integral<T>::type,
        bool
    >::type
    equal(const T& a,const T& b){
        return (a == b);
    }

}// arithmetic_tools
}// boost

#endif