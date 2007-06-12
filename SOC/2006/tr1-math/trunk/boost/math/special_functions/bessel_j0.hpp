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

#ifndef BOOST_MATH_BESSEL_J0_HPP
#define BOOST_MATH_BESSEL_J0_HPP

#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/assert.hpp>

// Bessel function of the first kind of order zero
// x <= 8, minimax rational approximations on root-bracketing intervals
// x > 8, Hankel asymptotic expansion in Hart, Computer Approximations, 1968

namespace boost { namespace math {

template <typename T>
T bessel_j0(T x)
{
    static const T P1[] = {
        -4.1298668500990866786e+11L,
         2.7282507878605942706e+10L,
        -6.2140700423540120665e+08L,
         6.6302997904833794242e+06L,
        -3.6629814655107086448e+04L,
         1.0344222815443188943e+02L,
        -1.2117036164593528341e-01L,
    };
    static const T Q1[] = {
         2.3883787996332290397e+12L,
         2.6328198300859648632e+10L,
         1.3985097372263433271e+08L,
         4.5612696224219938200e+05L,
         9.3614022392337710626e+02L,
         1.0L,
         0.0L,
    };
    static const T P2[] = {
        -1.8319397969392084011e+03L,
        -1.2254078161378989535e+04L,
        -7.2879702464464618998e+03L,
         1.0341910641583726701e+04L,
         1.1725046279757103576e+04L,
         4.4176707025325087628e+03L,
         7.4321196680624245801e+02L,
         4.8591703355916499363e+01L,
    };
    static const T Q2[] = {
        -3.5783478026152301072e+05L,
         2.4599102262586308984e+05L,
        -8.4055062591169562211e+04L,
         1.8680990008359188352e+04L,
        -2.9458766545509337327e+03L,
         3.3307310774649071172e+02L,
        -2.5258076240801555057e+01L,
         1.0L,
    };
    static const T PC[] = {
         2.2779090197304684302e+04L,
         4.1345386639580765797e+04L,
         2.1170523380864944322e+04L,
         3.4806486443249270347e+03L,
         1.5376201909008354296e+02L,
         8.8961548424210455236e-01L,
    };
    static const T QC[] = {
         2.2779090197304684318e+04L,
         4.1370412495510416640e+04L,
         2.1215350561880115730e+04L,
         3.5028735138235608207e+03L,
         1.5711159858080893649e+02L,
         1.0L,
    };
    static const T PS[] = {
        -8.9226600200800094098e+01L,
        -1.8591953644342993800e+02L,
        -1.1183429920482737611e+02L,
        -2.2300261666214198472e+01L,
        -1.2441026745835638459e+00L,
        -8.8033303048680751817e-03L,
    };
    static const T QS[] = {
         5.7105024128512061905e+03L,
         1.1951131543434613647e+04L,
         7.2642780169211018836e+03L,
         1.4887231232283756582e+03L,
         9.0593769594993125859e+01L,
         1.0L,
    };
    static const T x1  =  2.4048255576957727686e+00L,
                   x2  =  5.5200781102863106496e+00L,
                   x11 =  6.160e+02L,
                   x12 = -1.42444230422723137837e-03L,
                   x21 =  1.4130e+03L,
                   x22 =  5.46860286310649596604e-04L
    ;
    T value, factor, r, rc, rs;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (x < 0)
    {
        x = -x;                         // even function
    }
    if (x == 0)
    {
        return static_cast<T>(1);
    }
    if (x <= 4.0)                       // x in (0, 4]
    {
        T y = x * x;
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        r = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        factor = (x + x1) * ((x - x11/256.0L) - x12);
        value = factor * r;
    }
    else if (x <= 8.0)                  // x in (4, 8]
    {
        T y = 1.0L - (x * x)/64.0L;
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = (x + x2) * ((x - x21/256.0L) - x22);
        value = factor * r;
    }
    else                                // x in (8, \infty)
    {
        T y = 8.0L / x;
        T y2 = y * y;
        T z = x - 0.25L * pi<T>();
        BOOST_ASSERT(sizeof(PC) == sizeof(QC));
        BOOST_ASSERT(sizeof(PS) == sizeof(QS));
        rc = evaluate_rational(PC, QC, y2, sizeof(PC)/sizeof(PC[0]));
        rs = evaluate_rational(PS, QS, y2, sizeof(PS)/sizeof(PS[0]));
        factor = sqrt(2.0L / (x * pi<T>()));
        value = factor * (rc * cos(z) - y * rs * sin(z));
    }

    return value;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_J0_HPP
