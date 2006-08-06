//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_1_HPP
#define BOOST_MATH_ELLINT_1_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/special_functions/ellint_rf.hpp>
#include <boost/math/tools/error_handling.hpp>

// Elliptic integrals (complete and incomplete) of the first kind
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

// Elliptic integral (Legendre form) of the first kind
template <typename T>
T ellint_f(T phi, T k)
{
    using namespace std;
    using namespace boost::math::tools;

    if (abs(k) > 1)
    {
        domain_error<T>("boost::math::ellint_f(phi, k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (abs(k) == 1)
    {
        cout << "Warning: boost::math::ellint_f, |k| = 1, "
             << "may result in boost::math::ellint_rf domain error"
             << endl;
    }

    T x = cos(phi) * cos(phi);
    T y = 1.0L - k * k * sin(phi) * sin(phi);
    T z = 1.0L;
    T value = sin(phi) * ellint_rf(x, y, z);

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
        domain_error<T>("boost::math::ellint_k(k)",
            "domain error, |k| > 1, function requires |k| <= 1");
    }
    if (abs(k) == 1)
    {
        cout << "Warning: boost::math::ellint_k, |k| = 1, "
             << "may result in boost::math::ellint_rf domain error"
             << endl;
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
