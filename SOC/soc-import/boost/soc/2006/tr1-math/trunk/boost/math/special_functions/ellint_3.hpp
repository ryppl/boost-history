//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_3_HPP
#define BOOST_MATH_ELLINT_3_HPP

#undef max      // avoid msvc macro conflict, gcc has no such problem

#include <boost/math/special_functions/ellint_rf.hpp>
#include <boost/math/special_functions/ellint_rj.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/error_handling.hpp>

// Elliptic integrals (complete and incomplete) of the third kind
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

// Elliptic integral (Legendre form) of the third kind
template <typename T>
T ellint_pi(T v, T phi, T k)
{
    T value, aphi, w, x, y, z, p, t;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (abs(k) > 1)
    {
        return domain_error<T>("boost::math::ellint_pi(v, phi, k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (v > 1)
    {
        cout << "Warning: boost::math::ellint_pi, v > 1, integral is singular, "
             << "Cauchy principal value is returned, but the result correctness "
             << "is not guaranteed, use at your own risk"
             << endl;
    }

    // Carlson's algorithm works only for |phi| <= 1/2,
    // use the integrand's periodicity to normalize phi
    aphi = abs(phi);
    w = 2.0L * aphi / pi<T>();
    if (w > std::numeric_limits<long long>::max())
    {
        value = w * ellint_pi(v, k);        // ignore normalization for huge w
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
        t = sin(aphi) * sin(aphi);
        y = 1.0L - k * k * t;
        z = 1.0L;
        p = 1.0L - v * t;
        value = sign * sin(aphi) * (ellint_rf(x, y, z) + v * t * ellint_rj(x, y, z, p) / 3.0L);
        value += (n == 0) ? 0.0L : n * ellint_pi(v, k);
    }

    if (phi < 0)
    {
        value *= -1.0L;                     // odd function
    }
    return value;
}

// Complete elliptic integral (Legendre form) of the third kind
template <typename T>
T ellint_pi(T v, T k)
{
    using namespace std;
    using namespace boost::math::tools;

    if (abs(k) > 1)
    {
        return domain_error<T>("boost::math::ellint_pi(v, k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (v > 1)
    {
        cout << "Warning: boost::math::ellint_pi, v > 1, integral is singular, "
             << "Cauchy principal value is returned, but the result correctness "
             << "is not guaranteed, use at your own risk"
             << endl;
    }
    if (abs(k) == 1 || v == 1)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            return std::numeric_limits<T>::infinity();
        }
        else
        {
            overflow_error<T>("boost::math::ellint_pi(v, k)",
                              "infinity occurred but not supported");
        }
    }

    T x = 0.0L;
    T y = 1.0L - k * k;
    T z = 1.0L;
    T p = 1.0L - v;
    T value = ellint_rf(x, y, z) + v * ellint_rj(x, y, z, p) / 3.0L;

    return value;
}

// -------------------- TR1 functions --------------------

inline float ellint_3f(float k, float nu, float phi)
{
    return ellint_pi<float>(nu, phi, k);
}

inline double ellint_3(double k, double nu, double phi)
{
    return ellint_pi<double>(nu, phi, k);
}

inline long double ellint_3l(long double k, long double nu, long double phi)
{
    return ellint_pi<long double>(nu, phi, k);
}

inline float comp_ellint_3f(float k, float nu)
{
    return ellint_pi<float>(nu, k);
}

inline double comp_ellint_3(double k, double nu)
{
    return ellint_pi<double>(nu, k);
}

inline long double comp_ellint_3l(long double k, long double nu)
{
    return ellint_pi<long double>(nu, k);
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_3_HPP
