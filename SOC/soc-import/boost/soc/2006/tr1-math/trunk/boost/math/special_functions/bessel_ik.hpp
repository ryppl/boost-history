//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_IK_HPP
#define BOOST_MATH_BESSEL_IK_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#define MAX_ITERATION 100000
#undef min      // avoid msvc macro conflict, gcc has no such problem

#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/error_handling.hpp>

// Modified Bessel functions of the first and second kind of fractional order

namespace boost { namespace math {

namespace bessel_ik_detail {

// Calculate K(v, x) and K(v+1, x) by method analogous to
// Temme, Journal of Computational Physics, vol 21, 343 (1976)
template <typename T>
int temme(T v, T x, T* K, T* K1)
{
    T f, h, p, q, coef, sum, sum1, tolerance;
    T a, b, c, d, sigma, gamma1, gamma2;
    int k;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (abs(v) > 0.5)
    {
        domain_error<T>("boost::math::temme(v, x, &K, &K1)",
            "domain error, |v| too large, Temme series converge only for |v| <= 1/2");
    }
    // |x| <= 2, Temme series converge rapidly
    // |x| > 2, the larger the |x|, the slower the convergence
    if (abs(x) > 2)
    {
        cout << "Warning: boost::math::temme, convergence may fail for large |x|" << endl;
    }

    a = log(0.5L * x);
    b = exp(v * a);
    sigma = -a * v;
    c = abs(v) < std::numeric_limits<T>::epsilon() ?
        1.0L : sin(v * pi<T>()) / (v * pi<T>());
    d = abs(sigma) < std::numeric_limits<T>::epsilon() ?
        1.0L : sinh(sigma) / sigma;
    gamma1 = abs(v) < std::numeric_limits<T>::epsilon() ?
        -euler<T>() : (0.5L / v) * (tgamma(1.0L + v) - tgamma(1.0L - v)) * c;
    gamma2 = 0.5L * (tgamma(1.0L + v) + tgamma(1.0L - v)) * c;

    // initial values
    p = 0.5L * (tgamma(1.0L + v) / b);
    q = 0.5L * (tgamma(1.0L - v) * b);
    f = (cosh(sigma) * gamma1 + d * (-a) * gamma2) / c;
    h = p;
    coef = 1.0L;
    sum = coef * f;
    sum1 = coef * h;

    // series summation
    tolerance = std::numeric_limits<T>::epsilon();
    for (k = 1; k < MAX_ITERATION; k++)
    {
        f = (k * f + p + q) / (k*k - v*v);
        p /= k - v;
        q /= k + v;
        h = p - k * f;
        coef *= 0.25L * x * x / k;
        sum += coef * f;
        sum1 += coef * h;
        if (abs(coef * f) < abs(sum) * tolerance) { break; }
    }
    if (k >= MAX_ITERATION)
    {
        cout << "Warning: boost::math::temme failed to converge" << endl;
    }
    *K = sum;
    *K1 = 2.0L * sum1 / x;

    return 0;
}

// Evaluate continued fraction fv = I_(v+1) / I_v, derived from
// Abramowitz and Stegun, Handbook of Mathematical Functions, 1972, 9.1.73
template <typename T>
int CF1(T v, T x, T* fv)
{
    T C, D, f, a, b, delta, tiny, tolerance;
    int k;

    using namespace std;

    // |x| <= |v|, CF1 converges rapidly
    // |x| > |v|, CF1 needs O(|x|) iterations to converge
    if (abs(x) > abs(v) && abs(x) > 0.5L * MAX_ITERATION)
    {
        cout << "Warning: boost::math::CF1, |x| too large, convergence may fail" << endl;
    }

    // modified Lentz's method, see
    // Lentz, Applied Optics, vol 15, 668 (1976)
    tolerance = 2.0L * std::numeric_limits<T>::epsilon();
    tiny = sqrt(std::numeric_limits<T>::min());
    C = f = tiny;                           // b0 = 0, replace with tiny
    D = 0.0L;
    for (k = 1; k < MAX_ITERATION; k++)
    {
        a = 1.0L;
        b = 2.0L * (v + k) / x;
        C = b + a / C;
        D = b + a * D;
        if (C == 0) { C = tiny; }
        if (D == 0) { D = tiny; }
        D = 1.0L / D;
        delta = C * D;
        f *= delta;
        if (abs(delta - 1.0L) < tolerance) { break; }
    }
    if (k >= MAX_ITERATION)
    {
        cout << "Warning: boost::math::CF1 failed to converge" << endl;
    }
    *fv = f;

    return 0;
}

// Calculate K(v, x) and K(v+1, x) by evaluating continued fraction
// z1 / z0 = U(v+1.5, 2v+1, 2x) / U(v+0.5, 2v+1, 2x), see
// Thompson and Barnett, Computer Physics Communications, vol 47, 245 (1987)
template <typename T>
int CF2(T v, T x, T* Kv, T* Kv1)
{
    using namespace std;
    using namespace boost::math::constants;

    T S, C, Q, D, f, a, b, q, delta, tolerance, current, prev;
    int k;

    // |x| >= |v|, CF2 converges rapidly
    // |x| -> 0, CF2 fails to converge
    if (abs(x) < 1)
    {
        cout << "Warning: boost::math::CF2, |x| too small, convergence may fail" << endl;
    }

    // Steed's algorithm, see Thompson and Barnett,
    // Journal of Computational Physics, vol 64, 490 (1986)
    tolerance = std::numeric_limits<T>::epsilon();
    a = v * v - 0.25L;
    b = 2.0L * (x + 1.0L);                  // b1
    D = 1.0L / b;                           // D1 = 1 / b1
    f = delta = D;                          // f1 = delta1 = D1, coincidence
    prev = 0.0L;                            // q0
    current = 1.0L;                         // q1
    Q = C = -a;                             // Q1 = C1 because q1 = 1
    S = 1.0L + Q * delta;                   // S1
    for (k = 2; k < MAX_ITERATION; k++)     // starting from 2
    {
        // continued fraction f = z1 / z0
        a -= 2.0L * (k - 1);
        b += 2.0L;
        D = 1.0L / (b + a * D);
        delta *= b * D - 1.0L;
        f += delta;

        // series summation S = 1 + \sum_{n=1}^{\infty} C_n * z_n / z_0
        q = (prev - (b - 2.0L) * current) / a;
        prev = current;
        current = q;                        // forward recurrence for q
        C *= -a / k;
        Q += C * q;
        S += Q * delta;

        // S converges slower than f
        if (abs(Q * delta) < abs(S) * tolerance) { break; }
    }
    if (k >= MAX_ITERATION)
    {
        cout << "Warning: boost::math::CF2 failed to converge" << endl;
    }
    *Kv = sqrt(0.5L * pi<T>() / x) * exp(-x) / S;
    *Kv1 = *Kv * (0.5L + v + x + (v * v - 0.25L) * f) / x;

    return 0;
}

} // namespace bessel_ik_detail

// Compute I(v, x) and K(v, x) simultaneously by Temme's method, see
// Temme, Journal of Computational Physics, vol 19, 324 (1975)
template <typename T>
int bessel_ik(T v, T x, T* I, T* K)
{
    // Kv1 = K_(v+1), fv = I_(v+1) / I_v
    // Ku1 = K_(u+1), fu = I_(u+1) / I_u
    T u, Iv, Kv, Kv1, Ku, Ku1, fv;
    T W, current, prev, next;
    bool reflect = false;
    int n, k;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;
    using namespace boost::math::bessel_ik_detail;

    if (v < 0)
    {
        reflect = true;
        v = -v;                             // v is non-negative from here
    }
    n = static_cast<int>(v + 0.5L);
    u = v - n;                              // -1/2 <= u < 1/2

    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_ik(v, x, &I, &K)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            Iv = (v == 0) ? static_cast<T>(1) : static_cast<T>(0);
            Kv = std::numeric_limits<T>::infinity();
            goto reflection;
        }
        else
        {
            overflow_error<T>("boost::math::bessel_ik(v, x, &I, &K)",
                              "infinity occurred but not supported");
        }
    }

    // x is positive until reflection
    W = 1.0L / x;                           // Wronskian
    if (x <= 2.0)                           // x in (0, 2]
    {
        temme(u, x, &Ku, &Ku1);             // Temme series
    }
    else                                    // x in (2, \infty)
    {
        CF2(u, x, &Ku, &Ku1);               // continued fraction CF2
    }
    prev = Ku;
    current = Ku1;
    for (k = 1; k <= n; k++)                // forward recurrence for K
    {
        next = 2.0L * (u + k) * current / x + prev;
        prev = current;
        current = next;
    }
    Kv = prev;
    Kv1 = current;
    CF1(v, x, &fv);                         // continued fraction CF1
    Iv = W / (Kv * fv + Kv1);               // Wronskian relation

reflection:
    if (reflect)
    {
        T z = (u + n % 2) * pi<T>();
        *I = Iv + (2.0L / pi<T>()) * sin(z) * Kv;   // reflection formula
        *K = Kv;
    }
    else
    {
        *I = Iv;
        *K = Kv;
    }

    return 0;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_IK_HPP
