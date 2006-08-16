//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_RD_HPP
#define BOOST_MATH_ELLINT_RD_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#define MAX_ITERATION 100000
#define MAX3(a, b, c) ( (a)>(b) ? (a)>(c)?(a):(c) : (b)>(c)?(b):(c) )

#include <boost/math/tools/error_handling.hpp>

// Carlson's elliptic integral of the second kind
// R_D(x, y, z) = R_J(x, y, z, z) = 1.5 * \int_{0}^{\infty} [(t+x)(t+y)]^{-1/2} (t+z)^{-3/2} dt
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

template <typename T>
T ellint_rd(T x, T y, T z)
{
    T value, u, lambda, sigma, factor, tolerance;
    T X, Y, Z, EA, EB, EC, ED, EE, S1, S2;
    int k;

    using namespace std;
    using namespace boost::math::tools;

    if (x < 0 || y < 0 || z <= 0)
    {
        return domain_error<T>("boost::math::ellint_rd(x, y, z)",
            "domain error, arguments x, y must be non-negative and z must be positive");
    }
    if (x + y == 0)
    {
        return domain_error<T>("boost::math::ellint_rd(x, y, z)",
            "domain error, at most one argument can be zero");
    }

    // error scales as the 6th power of tolerance
    tolerance = pow((1.0L/3.0L)*std::numeric_limits<T>::epsilon(), 1.0L/6.0L);

    // duplication
    sigma = 0.0L;
    factor = 1.0L;
    for (k = 1; k < MAX_ITERATION; k++)
    {
        u = (x + y + z + z + z) / 5.0L;
        X = 1.0L - x / u;
        Y = 1.0L - y / u;
        Z = 1.0L - z / u;
        if (MAX3(abs(X), abs(Y), abs(Z)) < tolerance) { break; }
        lambda = sqrt(x * y) + sqrt(y * z) + sqrt(z * x);
        sigma += factor / (sqrt(z) * (z + lambda));
        factor *= 0.25L;
        x = 0.25L * (x + lambda);
        y = 0.25L * (y + lambda);
        z = 0.25L * (z + lambda);
    }
    if (k >= MAX_ITERATION)             // virtually impossible
    {
        cout << "Warning: boost::math::ellint_rd failed to converge" << endl;
    }

    // Taylor series expansion to the 5th order
    EA = X * Y;
    EB = Z * Z;
    EC = EA - EB;
    ED = EA - 6.0L * EB;
    EE = ED + EC + EC;
    S1 = ED*(ED*9.0L/88.0L - Z*EE*9.0L/52.0L - 3.0L/14.0L);
    S2 = Z*(EE/6.0L + Z*(-EC*9.0L/22.0L + Z*EA*3.0L/26.0L));
    value = 3.0L * sigma + factor * (1.0L + S1 + S2) / (u * sqrt(u));

    return value;
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_RD_HPP
