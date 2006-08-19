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

#ifndef BOOST_MATH_BESSEL_IN_HPP
#define BOOST_MATH_BESSEL_IN_HPP

#include <boost/math/special_functions/bessel_i0.hpp>
#include <boost/math/special_functions/bessel_i1.hpp>
#include <boost/math/special_functions/bessel_ik.hpp>

// Modified Bessel function of the first kind of integer order
// I_n(z) is the minimal solution, forward recurrence always unstable,
// use Miller's algorithm

namespace boost { namespace math {

template <typename T>
T bessel_in(int n, T x)
{
    T value, current, prev, next;

    using namespace std;

    if (n == 0)
    {
        return bessel_i0(x);
    }
    if (n == 1)
    {
        return bessel_i1(x);
    }
    if (n < 0)
    {
        n = -n;                             // I_{-n}(z) = I_n(z)
    }

    if (x == 0)                             // n >= 2
    {
        return static_cast<T>(0);
    }
    else                                    // backward recurrence
    {
        T fn;                               // fn = I_(n+1) / I_n
        // convergence speed for continued fraction CF1 not guaranteed
        boost::math::bessel_ik_detail::CF1(static_cast<T>(n), x, &fn);
        // tiny initial value to prevent overflow
        T init = sqrt(std::numeric_limits<T>::min());
        prev = fn * init;
        current = init;
        for (int k = n; k > 0; k--)
        {
            next = 2.0L * k * current / x + prev;
            prev = current;
            current = next;
        }
        T ratio = init / current;           // scaling ratio
        value = bessel_i0(x) * ratio;       // normalization
    }

    return value;
}

template <typename T>
inline T bessel_in(T n, T x)
{
    return bessel_in(static_cast<int>(n), x);
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_IN_HPP
