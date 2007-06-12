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

#ifndef BOOST_MATH_BESSEL_K0_HPP
#define BOOST_MATH_BESSEL_K0_HPP

#include <boost/math/tools/rational.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <boost/assert.hpp>

// Modified Bessel function of the second kind of order zero
// minimax rational approximations on intervals, see
// Russon and Blair, Chalk River Report AECL-3461, 1969

namespace boost { namespace math {

template <typename T>
T bessel_k0(T x)
{
    static const T P1[] = {
         2.4708152720399552679e+03L,
         5.9169059852270512312e+03L,
         4.6850901201934832188e+02L,
         1.1999463724910714109e+01L,
         1.3166052564989571850e-01L,
         5.8599221412826100000e-04L,
    };
    static const T Q1[] = {
         2.1312714303849120380e+04L,
        -2.4994418972832303646e+02L,
         1.0L,
         0.0L,
         0.0L,
         0.0L,
    };
    static const T P2[] = {
        -1.6128136304458193998e+06L,
        -3.7333769444840079748e+05L,
        -1.7984434409411765813e+04L,
        -2.9501657892958843865e+02L,
        -1.6414452837299064100e+00L,
    };
    static const T Q2[] = {
        -1.6128136304458193998e+06L,
         2.9865713163054025489e+04L,
        -2.5064972445877992730e+02L,
         1.0L,
         0.0L,
    };
    static const T P3[] = {
         1.1600249425076035558e+02L,
         2.3444738764199315021e+03L,
         1.8321525870183537725e+04L,
         7.1557062783764037541e+04L,
         1.5097646353289914539e+05L,
         1.7398867902565686251e+05L,
         1.0577068948034021957e+05L,
         3.1075408980684392399e+04L,
         3.6832589957340267940e+03L,
         1.1394980557384778174e+02L,
         0.0L,
    };
    static const T Q3[] = {
         9.2556599177304839811e+01L,
         1.8821890840982713696e+03L,
         1.4847228371802360957e+04L,
         5.8824616785857027752e+04L,
         1.2689839587977598727e+05L,
         1.5144644673520157801e+05L,
         9.7418829762268075784e+04L,
         3.1474655750295278825e+04L,
         4.4329628889746408858e+03L,
         2.0013443064949242491e+02L,
         1.0L,
    };
    T value, factor, r, r1, r2;

    using namespace std;
    using namespace boost::math::tools;

    if (x < 0)
    {
        return domain_error<T>("boost::math::bessel_k0(x)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        return overflow_error<T>("boost::math::bessel_k0(x)",
            "infinity occurred but not supported");
    }
    if (x <= 1.0)                       // x in (0, 1]
    {
        T y = x * x;
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r1 = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        r2 = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = log(x);
        value = r1 - factor * r2;
    }
    else                                // x in (1, \infty)
    {
        T y = 1.0L / x;
        BOOST_ASSERT(sizeof(P3) == sizeof(Q3));
        r = evaluate_rational(P3, Q3, y, sizeof(P3)/sizeof(P3[0]));
        factor = exp(-x) / sqrt(x);
        value = factor * r;
    }

    return value;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_K0_HPP
