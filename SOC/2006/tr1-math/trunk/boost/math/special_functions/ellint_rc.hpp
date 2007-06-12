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

#ifndef BOOST_MATH_ELLINT_RC_HPP
#define BOOST_MATH_ELLINT_RC_HPP

#define MAX_ITERATION 100000

#include <boost/math/tools/error_handling.hpp>

// Carlson's degenerate elliptic integral
// R_C(x, y) = R_F(x, y, y) = 0.5 * \int_{0}^{\infty} (t+x)^{-1/2} (t+y)^{-1} dt
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

template <typename T>
T ellint_rc(T x, T y)
{
    T value, S, u, lambda, tolerance;
    int k;

    using namespace std;
    using namespace boost::math::tools;

    if (x < 0 || y == 0)
    {
        return domain_error<T>("boost::math::ellint_rc(x, y)",
            "domain error, argument x must be non-negative and y must be nonzero");
    }

    // error scales as the 6th power of tolerance
    tolerance = pow(4.0L*std::numeric_limits<T>::epsilon(), 1.0L/6.0L);

    // for y < 0, the integral is singular, return Cauchy principal value
    if (y < 0)
    {
        x = x - y;
        y = -y;
    }

    // duplication
    for (k = 1; k < MAX_ITERATION; k++)
    {
        u = (x + y + y) / 3.0L;
        S = y / u - 1.0L;               // 1 - x / u = 2 * S
        if (2.0L * abs(S) < tolerance) { break; }
        lambda = 2.0L * sqrt(x * y) + y;
        x = 0.25L * (x + lambda);
        y = 0.25L * (y + lambda);
    }
    if (k >= MAX_ITERATION)             // virtually impossible
    {
        cout << "Warning: boost::math::ellint_rc failed to converge" << endl;
    }

    // Taylor series expansion to the 5th order
    value = (1.0L + S*S*(0.3L + S*(1.0L/7.0L + S*(0.375L + S*9.0L/22.0L)))) / sqrt(u);

    if (y < 0)
    {
        value *= sqrt(1.0L - y / x);
    }
    return value;
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_RC_HPP
