//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_JY_HPP
#define BOOST_MATH_BESSEL_JY_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#define MAX_ITERATION 10000

#include <boost/math/special_functions/bessel_jn.hpp>
#include <boost/math/special_functions/bessel_yn.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <complex>

// Bessel functions of the first and second kind of fractional order

namespace boost { namespace math {

// Calculate Y(v, x) and Y(v+1, x) by Temme's method, see
// Temme, Journal of Computational Physics, vol 21, 343 (1976)
template <typename T>
int temme(T v, T x, T* Y, T* Y1)
{
    T g, h, p, q, f, coef, sum, sum1, tolerance;
    T a, b, c, d, e, sigma, gamma1, gamma2;
    int k;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (abs(v) > 0.5)
    {
        domain_error<T>("boost::math::temme(v, x, &Y, &Y1)",
            "domain error, |v| too large, Temme series converge only for |v| <= 1/2");
    }

    a = log(0.5L * x);
    b = exp(v * a);
    sigma = -a * v;
    c = abs(v) < std::numeric_limits<T>::epsilon() ?
        1.0L : sin(v * pi<T>()) / (v * pi<T>());
    d = abs(sigma) < std::numeric_limits<T>::epsilon() ?
        1.0L : sinh(sigma) / sigma;
    e = 2.0L * sin(0.5L * v * pi<T>()) * (sin(0.5L * v * pi<T>()) / v);
    gamma1 = (0.5L / v) * (tgamma(1.0L + v) - tgamma(1.0L - v)) * c;
    gamma2 = 0.5L * (tgamma(1.0L + v) + tgamma(1.0L - v)) * c;

    // initial values
    p = (tgamma(1.0L + v) / b) / pi<T>();
    q = (tgamma(1.0L - v) * b) / pi<T>();
    f = 2.0L * (cosh(sigma) * gamma1 + d * (-a) * gamma2) / (c * pi<T>());
    g = f + e * q;
    h = p;
    coef = 1.0L;
    sum = g;
    sum1 = h;

    // series summation
    tolerance = 1.0e+02L * std::numeric_limits<T>::epsilon();
    for (k = 1; k < MAX_ITERATION; k++)
    {
        f = (k * f + p + q) / (k*k - v*v);
        p /= k - v;
        q /= k + v;
        g = f + e * q;
        h = p - k * g;
        coef *= -0.25L * x * x / k;
        sum += coef * g;
        sum1 += coef * h;
        if (abs(coef * g) < (1.0L + abs(sum)) * tolerance) { break; }
    }
    if (k >= MAX_ITERATION)
    {
        // failed to converge warning
    }
    *Y = -sum;
    *Y1 = -2.0L * sum1 / x;

    return 0;
}

// Evaluate continued fraction fv = J_(v+1) / J_v, see
// Abramowitz and Stegun, Handbook of Mathematical Functions, 1972
template <typename T>
int CF1(T v, T x, T* fv, int* sign)
{
    T C, D, f, a, b, delta, tiny, tolerance;
    int k, s = 1;

    using namespace std;

    // modified Lentz's method, see
    // Lentz, Applied Optics, vol 15, 668 (1976)
    tolerance = 1.0e+02L * std::numeric_limits<T>::epsilon();
    tiny = sqrt(std::numeric_limits<T>::min());
    C = f = tiny;                           // b0 = 0, replace with tiny
    D = 0.0L;
    for (k = 1; k < MAX_ITERATION; k++)
    {
        a = -1.0L;
        b = 2.0L * (v + k) / x;
        C = b + a / C;
        D = b + a * D;
        if (C == 0) { C = tiny; }
        if (D == 0) { D = tiny; }
        D = 1.0L / D;
        delta = C * D;
        f *= delta;
        if (D < 0) { s = -s; }
        if (abs(delta - 1.0L) < tolerance) { break; }
    }
    if (k >= MAX_ITERATION)
    {
        // failed to converge warning
    }
    *fv = -f;
    *sign = s;                              // sign of denominator

    return 0;
}

// Evaluate continued fraction p + iq = (J' + iY') / (J + iY), see
// Press et al, Numerical Recipes in C, 2nd edition, 1992
template <typename T>
int CF2(T v, T x, T* p, T* q)
{
    using namespace std;

    complex<T> C, D, f, a, b, delta, tiny;
    int k;

    // modified Lentz's method, complex numbers involved, see
    // Lentz, Applied Optics, vol 15, 668 (1976)
    T tolerance = 1.0e+02L * std::numeric_limits<T>::epsilon();
    tiny = sqrt(std::numeric_limits<T>::min());
    C = f = complex<T>(-0.5L/x, 1.0L);
    D = 0.0L;
    for (k = 1; k < MAX_ITERATION; k++)
    {
        a = (k - 0.5L)*(k - 0.5L) - v*v;
        if (k == 1)
        {
            a *= complex<T>(0.0L, 1.0L/x);
        }
        b = complex<T>(2.0L*x, 2.0L*k);
        C = b + a / C;
        D = b + a * D;
        if (C == 0.0L) { C = tiny; }
        if (D == 0.0L) { D = tiny; }
        D = 1.0L / D;
        delta = C * D;
        f *= delta;
        if (abs(delta - 1.0L) < tolerance) { break; }
    }
    if (k >= MAX_ITERATION)
    {
        // failed to converge warning
    }
    *p = real(f);
    *q = imag(f);

    return 0;
}

// Compute J(v, x) and Y(v, x) simultaneously by Steed's method, see
// Barnett et al, Computer Physics Communications, vol 8, 377 (1974)
template <typename T>
int bessel_jy(T v, T x, T* J, T* Y)
{
    // Jv1 = J_(v+1), Yv1 = Y_(v+1), fv = J_(v+1) / J_v
    // Ju1 = J_(u+1), Yu1 = Y_(u+1), fu = J_(u+1) / J_u
    T u, Jv, Jv1, Ju, Ju1, Yv, Yv1, Yu, Yu1, fv, fu;
    T W, p, q, gamma, current, prev, next;
    bool reflect = false;
    int n, k, s;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (v < 0)
    {
        reflect = true;
        v = -v;
    }
    n = static_cast<int>(v + 0.5L);
    u = v - n;                              // -1/2 <= u < 1/2
    if (u == 0)
    {
        *J = bessel_jn(n, x);               // v is integer
        *Y = bessel_yn(n, x);
        return 0;
    }

    // v is non-negative and non-integer from here
    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_jy(v, x, &J, &Y)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            Jv = static_cast<T>(0);
            Yv = -std::numeric_limits<T>::infinity();
            goto reflection;
        }
        else
        {
            overflow_error<T>("boost::math::bessel_jy(v, x, &J, &Y)",
                              "infinity occurred but not supported");
        }
    }

    // x is positive until reflection
    W = 2.0L / (x * pi<T>());               // Wronskian
    if (x <= 2.0)                           // x in (0, 2]
    {
        temme(u, x, &Yu, &Yu1);             // Temme series
        prev = Yu;
        current = Yu1;
        for (k = 1; k <= n; k++)            // forward recurrence for Y
        {
            next = 2.0L * (u + k) * current / x - prev;
            prev = current;
            current = next;
        }
        Yv = prev;
        Yv1 = current;
        CF1(v, x, &fv, &s);                 // continued fraction
        Jv = W / (Yv * fv - Yv1);           // Wronskian relation
    }
    else                                    // x in (2, \infty)
    {
        CF1(v, x, &fv, &s);
        // tiny initial value to prevent overflow
        T init = sqrt(std::numeric_limits<T>::min());
        prev = fv * s * init;
        current = s * init;
        for (k = n; k > 0; k--)             // backward recurrence for J
        {
            next = 2.0L * (u + k) * current / x - prev;
            prev = current;
            current = next;
        }
        fu = prev / current;
//compare fu to CF1(u, x, &fu, &s)?
//get rid of s?
        CF2(u, x, &p, &q);                  // continued fraction
        gamma = (p - fu) / q;
        Ju = s * sqrt(W / (q + gamma * (p - fu)));
        T ratio = (s * init) / current;     // scaling ratio
        Jv = Ju * ratio;                    // normalization

        // compute Y
        Yu = gamma * Ju;
        Yu1 = Yu * (u/x - p - q/gamma);
        prev = Yu;
        current = Yu1;
        for (k = 1; k <= n; k++)            // forward recurrence for Y
        {
            next = 2.0L * (u + k) * current / x - prev;
            prev = current;
            current = next;
        }
        Yv = prev;
    }

reflection:
    if (reflect)
    {
        T z = v * pi<T>();
        *J = cos(z) * Jv - sin(z) * Yv;     // reflection formula
        *Y = sin(z) * Jv + cos(z) * Yv;
    }
    else
    {
        *J = Jv;
        *Y = Yv;
    }

    return 0;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_JY_HPP
