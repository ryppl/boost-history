//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_3_HPP
#define BOOST_MATH_ELLINT_3_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/special_functions/ellint_rf.hpp>
#include <boost/math/special_functions/ellint_rj.hpp>
#include <boost/math/tools/error_handling.hpp>

// Elliptic integrals (complete and incomplete) of the third kind
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

// Elliptic integral (Legendre form) of the third kind
template <typename T>
T ellint_pi(T v, T phi, T k)
{
    using namespace std;
    using namespace boost::math::tools;

    if (abs(k) > 1)
    {
        domain_error<T>("boost::math::ellint_pi(v, phi, k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (abs(k) == 1)
    {
        cout << "Warning: boost::math::ellint_pi, |k| = 1, "
             << "may result in boost::math::ellint_rf "
             << "or boost::math::ellint_rj domain error"
             << endl;
    }

    T x = cos(phi) * cos(phi);
    T t = sin(phi) * sin(phi);
    T y = 1.0L - k * k * t;
    T z = 1.0L;
    T p = 1.0L - v * t;
    T value = sin(phi) * (ellint_rf(x, y, z) + v * t * ellint_rj(x, y, z, p) / 3.0L);

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
        domain_error<T>("boost::math::ellint_pi(v, k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (abs(k) == 1)
    {
        cout << "Warning: boost::math::ellint_pi, |k| = 1, "
             << "may result in boost::math::ellint_rf "
             << "or boost::math::ellint_rj domain error"
             << endl;
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
