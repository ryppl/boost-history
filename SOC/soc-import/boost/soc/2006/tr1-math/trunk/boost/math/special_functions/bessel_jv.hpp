//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_JV_HPP
#define BOOST_MATH_BESSEL_JV_HPP

#include <boost/math/special_functions/bessel_jy.hpp>

// Bessel function of the first kind of fractional order

namespace boost { namespace math {

inline float bessel_jv(float v, float x)
{
    float J, Y;
    bessel_jy<float>(v, x, &J, &Y);
    return J;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_JV_HPP
