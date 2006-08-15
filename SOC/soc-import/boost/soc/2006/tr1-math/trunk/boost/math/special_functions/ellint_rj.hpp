//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_RJ_HPP
#define BOOST_MATH_ELLINT_RJ_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#define MAX_ITERATION 100000
#define MAX2(a, b) ( (a) > (b) ? (a) : (b) )
#define MAX4(a, b, c, d) ( MAX2(MAX2(a, b), MAX2(c, d)) )

#include <boost/math/special_functions/ellint_rc.hpp>
#include <boost/math/tools/error_handling.hpp>

// Carlson's elliptic integral of the third kind
// R_J(x, y, z, p) = 1.5 * \int_{0}^{\infty} (t+p)^{-1} [(t+x)(t+y)(t+z)]^{-1/2} dt
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

namespace ellint_rj_detail {

template <typename T>
int swap(T& x, T& y)
{
    T t;
    t = x;
    x = y;
    y = t;
    return 0;
}

// Sort three items in place in ascending order
template <typename T>
int sort3(T& x, T& y, T& z)
{
    if (x > y) { swap(x, y); }
    if (x > z) { swap(x, z); }
    if (y > z) { swap(y, z); }
    return 0;
}

} // namespace ellint_rj_detail

template <typename T>
T ellint_rj(T x, T y, T z, T p)
{
    T value, u, lambda, alpha, beta, sigma, factor, tolerance;
    T X, Y, Z, P, EA, EB, EC, E2, E3, S1, S2, S3;
    T a, b, rho, tau;
    int k;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::ellint_rj_detail;

    if (x < 0 || y < 0 || z < 0 || p == 0)
    {
        domain_error<T>("boost::math::ellint_rj(x, y, z, p)",
            "domain error, arguments x, y, z must be non-negative and p must be nonzero");
    }
    if (x + y == 0 || y + z == 0 || z + x == 0)
    {
        domain_error<T>("boost::math::ellint_rj(x, y, z, p)",
            "domain error, at most one argument can be zero");
    }

    // error scales as the 6th power of tolerance
    tolerance = pow((1.0L/3.0L)*std::numeric_limits<T>::epsilon(), 1.0L/6.0L);

    // for p < 0, the integral is singular, return Cauchy principal value
    if (p < 0)
    {
        sort3(x, y, z);
        a = 1.0L / (y - p);
        b = a * (z - y) * (y - x);
        rho = x * z / y;
        tau = p * (1.0L + b / y);
        p = y + b;
    }

    // duplication
    sigma = 0.0L;
    factor = 1.0L;
    for (k = 1; k < MAX_ITERATION; k++)
    {
        u = (x + y + z + p + p) / 5.0L;
        X = 1.0L - x / u;
        Y = 1.0L - y / u;
        Z = 1.0L - z / u;
        P = 1.0L - p / u;
        if (MAX4(abs(X), abs(Y), abs(Z), abs(P)) < tolerance) { break; }
        lambda = sqrt(x * y) + sqrt(y * z) + sqrt(z * x);
        alpha = p * (sqrt(x) + sqrt(y) + sqrt(z)) + sqrt(x * y * z);
        alpha *= alpha;
        beta = p * (p + lambda) * (p + lambda);
        sigma += factor * ellint_rc(alpha, beta);
        factor *= 0.25L;
        x = 0.25L * (x + lambda);
        y = 0.25L * (y + lambda);
        z = 0.25L * (z + lambda);
        p = 0.25L * (p + lambda);
    }
    if (k >= MAX_ITERATION)             // virtually impossible
    {
        cout << "Warning: boost::math::ellint_rj failed to converge" << endl;
    }

    // Taylor series expansion to the 5th order
    EA = X * Y + Y * Z + Z * X;
    EB = X * Y * Z;
    EC = P * P;
    E2 = EA - 3.0L * EC;
    E3 = EB + 2.0L * P * (EA - EC);
    S1 = 1.0L + E2*(E2*9.0L/88.0L - E3*9.0L/52.0L - 3.0L/14.0L);
    S2 = EB*(1.0L/6.0L + P*(-6.0L/22.0L + P*3.0L/26.0L));
    S3 = P*((EA - EC)/3.0L - P*EA*3.0L/22.0L);
    value = 3.0L * sigma + factor * (S1 + S2 + S3) / (u * sqrt(u));

    if (p < 0)
    {
        value = a*(b*value + 3.0L*(ellint_rc(rho, tau) - ellint_rf(x, y, z)));
    }
    return value;
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_RJ_HPP
