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

#ifndef BOOST_MATH_BESSEL_I1_HPP
#define BOOST_MATH_BESSEL_I1_HPP

#include <boost/math/tools/rational.hpp>
#include <boost/assert.hpp>

// Modified Bessel function of the first kind of order one
// minimax rational approximations on intervals, see
// Blair and Edwards, Chalk River Report AECL-4928, 1974

namespace boost { namespace math {

template <typename T>
T bessel_i1(T x)
{
    static const T P1[] = {
        -1.4577180278143463643e+15L,
        -1.7732037840791591320e+14L,
        -6.9876779648010090070e+12L,
        -1.3357437682275493024e+11L,
        -1.4828267606612366099e+09L,
        -1.0588550724769347106e+07L,
        -5.1894091982308017540e+04L,
        -1.8225946631657315931e+02L,
        -4.7207090827310162436e-01L,
        -9.1746443287817501309e-04L,
        -1.3466829827635152875e-06L,
        -1.4831904935994647675e-09L,
        -1.1928788903603238754e-12L,
        -6.5245515583151902910e-16L,
        -1.9705291802535139930e-19L,
    };
    static const T Q1[] = {
        -2.9154360556286927285e+15L,
         9.7887501377547640438e+12L,
        -1.4386907088588283434e+10L,
         1.1594225856856884006e+07L,
        -5.1326864679904189920e+03L,
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
         1.4582087408985668208e-05L,
        -8.9359825138577646443e-04L,
         2.9204895411257790122e-02L,
        -3.4198728018058047439e-01L,
         1.3960118277609544334e+00L,
        -1.9746376087200685843e+00L,
         8.5591872901933459000e-01L,
        -6.0437159056137599999e-02L,
    };
    static const T Q2[] = {
         3.7510433111922824643e-05L,
        -2.2835624489492512649e-03L,
         7.4212010813186530069e-02L,
        -8.5017476463217924408e-01L,
         3.2593714889036996297e+00L,
        -3.8806586721556593450e+00L,
         1.0L,
         0.0L,
    };
    T value, factor, r, w;

    using namespace std;
    using namespace boost::math::tools;

    w = abs(x);
    if (x == 0)
    {
        return static_cast<T>(0);
    }
    if (w <= 15.0)                      // w in (0, 15]
    {
        T y = x * x;
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        r = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        factor = w;
        value = factor * r;
    }
    else                                // w in (15, \infty)
    {
        T y = 1.0L / w - 1.0L / 15.0L;
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = exp(w) / sqrt(w);
        value = factor * r;
    }

    if (x < 0)
    {
        value *= -1.0L;                 // odd function
    }
    return value;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_I1_HPP
