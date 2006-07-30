//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_JV_HPP
#define BOOST_MATH_BESSEL_JV_HPP

#include <boost/math/special_functions/bessel_jy.hpp>

// Bessel function of the first kind of fractional order

namespace boost { namespace math {

template <typename T>
inline T bessel_jv(T v, T x)
{
    T J, Y;
    bessel_jy(v, x, &J, &Y);
    return J;
}

inline float cyl_bessel_j(float nu, float x)
{
    return bessel_jv<float>(nu, x);
}

inline double cyl_bessel_j(double nu, double x)
{
    return bessel_jv<double>(nu, x);
}

inline long double cyl_bessel_j(long double nu, long double x)
{
    return bessel_jv<long double>(nu, x);
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_JV_HPP
