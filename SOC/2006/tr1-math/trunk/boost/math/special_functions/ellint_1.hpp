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

#ifndef BOOST_MATH_ELLINT_1_HPP
#define BOOST_MATH_ELLINT_1_HPP

#undef max      // avoid msvc macro conflict, gcc has no such problem

#include <boost/math/special_functions/ellint_rf.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/error_handling.hpp>

// Elliptic integrals (complete and incomplete) of the first kind
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

// Elliptic integral (Legendre form) of the first kind
template <typename T>
T ellint_f(T phi, T k)
{
    T value, aphi, w, x, y, z;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (abs(k) > 1)
    {
        return domain_error<T>("boost::math::ellint_f(phi, k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }

    // Carlson's algorithm works only for |phi| <= pi/2,
    // use the integrand's periodicity to normalize phi
    aphi = abs(phi);
    w = 2.0L * aphi / pi<T>();
    if (w > std::numeric_limits<long long>::max())
    {
        value = w * ellint_k(k);            // ignore normalization for huge w
    }
    else
    {
        long long n = static_cast<long long>(w), sign;
        if (n % 2 == 0)                     // n is even
        {
            aphi = aphi - n * 0.5L * pi<T>();
            sign = 1;
        }
        else                                // n is odd
        {
            n += 1;
            aphi = n * 0.5L * pi<T>() - aphi;
            sign = -1;
        }
        x = cos(aphi) * cos(aphi);
        y = 1.0L - k * k * sin(aphi) * sin(aphi);
        z = 1.0L;
        value = sign * sin(aphi) * ellint_rf(x, y, z);
        value += (n == 0) ? 0.0L : n * ellint_k(k);
    }

    if (phi < 0)
    {
        value *= -1.0L;                     // odd function
    }
    return value;
}

// Complete elliptic integral (Legendre form) of the first kind
template <typename T>
T ellint_k(T k)
{
    using namespace std;
    using namespace boost::math::tools;

    if (abs(k) > 1)
    {
        return domain_error<T>("boost::math::ellint_k(k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (abs(k) == 1)
    {
        return overflow_error<T>("boost::math::ellint_k(k)",
            "infinity occurred but not supported");
    }

    T x = 0.0L;
    T y = 1.0L - k * k;
    T z = 1.0L;
    T value = ellint_rf(x, y, z);

    return value;
}

// -------------------- TR1 functions --------------------

inline float ellint_1f(float k, float phi)
{
    return ellint_f<float>(phi, k);
}

inline double ellint_1(double k, double phi)
{
    return ellint_f<double>(phi, k);
}

inline long double ellint_1l(long double k, long double phi)
{
    return ellint_f<long double>(phi, k);
}

inline float comp_ellint_1f(float k)
{
    return ellint_k<float>(k);
}

inline double comp_ellint_1(double k)
{
    return ellint_k<double>(k);
}

inline long double comp_ellint_1l(long double k)
{
    return ellint_k<long double>(k);
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_1_HPP
