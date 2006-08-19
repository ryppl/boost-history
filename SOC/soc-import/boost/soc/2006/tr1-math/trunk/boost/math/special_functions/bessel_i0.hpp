//  Copyright (c) 2006 Xiaogang Zhang
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_I0_HPP
#define BOOST_MATH_BESSEL_I0_HPP

#include <boost/math/tools/rational.hpp>
#include <boost/assert.hpp>

// Modified Bessel function of the first kind of order zero
// minimax rational approximations on intervals, see
// Blair and Edwards, Chalk River Report AECL-4928, 1974

namespace boost { namespace math {

template <typename T>
T bessel_i0(T x)
{
    static const T P1[] = {
        -2.2335582639474375249e+15L,
        -5.5050369673018427753e+14L,
        -3.2940087627407749166e+13L,
        -8.4925101247114157499e+11L,
        -1.1912746104985237192e+10L,
        -1.0313066708737980747e+08L,
        -5.9545626019847898221e+05L,
        -2.4125195876041896775e+03L,
        -7.0935347449210549190e+00L,
        -1.5453977791786851041e-02L,
        -2.5172644670688975051e-05L,
        -3.0517226450451067446e-08L,
        -2.6843448573468483278e-11L,
        -1.5982226675653184646e-14L,
        -5.2487866627945699800e-18L,
    };
    static const T Q1[] = {
        -2.2335582639474375245e+15L,
         7.8858692566751002988e+12L,
        -1.2207067397808979846e+10L,
         1.0377081058062166144e+07L,
        -4.8527560179962773045e+03L,
         1.0L,
         0.0L,
         0.0L,
         0.0L,
         0.0L,
         0.0L,
         0.0L,
         0.0L,
         0.0L,
         0.0L,
    };
    static const T P2[] = {
        -2.2210262233306573296e-04L,
         1.3067392038106924055e-02L,
        -4.4700805721174453923e-01L,
         5.5674518371240761397e+00L,
        -2.3517945679239481621e+01L,
         3.1611322818701131207e+01L,
        -9.6090021968656180000e+00L,
         0.0L,
    };
    static const T Q2[] = {
        -5.5194330231005480228e-04L,
         3.2547697594819615062e-02L,
        -1.1151759188741312645e+00L,
         1.3982595353892851542e+01L,
        -6.0228002066743340583e+01L,
         8.5539563258012929600e+01L,
        -3.1446690275135491500e+01L,
         1.0L,
    };
    T value, factor, r;

    using namespace std;
    using namespace boost::math::tools;

    if (x < 0)
    {
        x = -x;                         // even function
    }
    if (x == 0)
    {
        return static_cast<T>(1);
    }
    if (x <= 15.0)                      // x in (0, 15]
    {
        T y = x * x;
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        value = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
    }
    else                                // x in (15, \infty)
    {
        T y = 1.0L / x - 1.0L / 15.0L;
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = exp(x) / sqrt(x);
        value = factor * r;
    }

    return value;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_I0_HPP
