//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_K1_HPP
#define BOOST_MATH_BESSEL_K1_HPP
#endif

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/tools/rational.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <boost/assert.hpp>

// Modified Bessel function of the second kind of order one
// minimax rational approximations on intervals, see
// Russon and Blair, Chalk River Report AECL-3461, 1969

namespace boost { namespace math {

template <typename T>
T bessel_k1(T x)
{
    static const T P1[] = {
        -2.2149374878243304548e+06L,
         7.1938920065420586101e+05L,
         1.7733324035147015630e+05L,
         7.1885382604084798576e+03L,
         9.9991373567429309922e+01L,
         4.8127070456878442310e-01L,
    };
    static const T Q1[] = {
        -2.2149374878243304548e+06L,
         3.7264298672067697862e+04L,
        -2.8143915754538725829e+02L,
         1.0L,
         0.0L,
         0.0L,
    };
    static const T P2[] = {
         0.0L,
        -1.3531161492785421328e+06L,
        -1.4758069205414222471e+05L,
        -4.5051623763436087023e+03L,
        -5.3103913335180275253e+01L,
        -2.2795590826955002390e-01L,
    };
    static const T Q2[] = {
        -2.7062322985570842656e+06L,
         4.3117653211351080007e+04L,
        -3.0507151578787595807e+02L,
         1.0L,
         0.0L,
         0.0L,
    };
    static const T P3[] = {
         2.2196792496874548962e+00L,
         4.4137176114230414036e+01L,
         3.4122953486801312910e+02L,
         1.3319486433183221990e+03L,
         2.8590657697910288226e+03L,
         3.4540675585544584407e+03L,
         2.3123742209168871550e+03L,
         8.1094256146537402173e+02L,
         1.3182609918569941308e+02L,
         7.5584584631176030810e+00L,
         6.4257745859173138767e-02L,
    };
    static const T Q3[] = {
         1.7710478032601086579e+00L,
         3.4552228452758912848e+01L,
         2.5951223655579051357e+02L,
         9.6929165726802648634e+02L,
         1.9448440788918006154e+03L,
         2.1181000487171943810e+03L,
         1.2082692316002348638e+03L,
         3.3031020088765390854e+02L,
         3.6001069306861518855e+01L,
         1.0L,
         0.0L,
    };
    T value, factor, r, r1, r2;

    using namespace std;
    using namespace boost::math::tools;

    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_k1(x)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            return std::numeric_limits<T>::infinity();
        }
        else
        {
            overflow_error<T>("boost::math::bessel_k1(x)",
                              "infinity occurred but not supported");
        }
    }
    if (x <= 1.0)                       // x in (0, 1]
    {
        T y = x * x;
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r1 = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        r2 = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = log(x);
        value = (r1 + factor * r2) / x;
    }
    else                                // x in (1, \infty)
    {
        T y = 1.0L / x;
        BOOST_ASSERT(sizeof(P3) == sizeof(Q3));
        r = evaluate_rational(P3, Q3, y, sizeof(P3)/sizeof(P3[0]));
        factor = exp(-x) / sqrt(x);
        value = factor * r;
    }

    return value;
}

}} // namespaces
