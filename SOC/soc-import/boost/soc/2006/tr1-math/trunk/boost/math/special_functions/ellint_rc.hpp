//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_RC_HPP
#define BOOST_MATH_ELLINT_RC_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

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
