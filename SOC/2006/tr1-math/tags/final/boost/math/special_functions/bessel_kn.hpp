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

#ifndef BOOST_MATH_BESSEL_KN_HPP
#define BOOST_MATH_BESSEL_KN_HPP

#include <boost/math/special_functions/bessel_k0.hpp>
#include <boost/math/special_functions/bessel_k1.hpp>
#include <boost/math/tools/error_handling.hpp>

// Modified Bessel function of the second kind of integer order
// K_n(z) is the dominant solution, forward recurrence always OK (though unstable)

namespace boost { namespace math {

template <typename T>
T bessel_kn(int n, T x)
{
    T value, current, prev;

    using namespace boost::math::tools;

    if (x < 0)
    {
        return domain_error<T>("boost::math::bessel_kn(n, x)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        return overflow_error<T>("boost::math::bessel_kn(n, x)",
            "infinity occurred but not supported");
    }

    if (n == 0)
    {
        return bessel_k0(x);
    }
    if (n == 1)
    {
        return bessel_k1(x);
    }
    if (n < 0)
    {
        n = -n;                             // K_{-n}(z) = K_n(z)
    }

    prev = bessel_k0(x);
    current = bessel_k1(x);
    for (int k = 1; k < n; k++)            // n >= 2
    {
        value = 2.0L * k * current / x + prev;
        prev = current;
        current = value;
    }

    return value;
}

template <typename T>
inline T bessel_kn(T n, T x)
{
    return bessel_kn(static_cast<int>(n), x);
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_KN_HPP
