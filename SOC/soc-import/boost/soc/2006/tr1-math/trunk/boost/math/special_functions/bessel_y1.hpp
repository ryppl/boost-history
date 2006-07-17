//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_Y1_HPP
#define BOOST_MATH_BESSEL_Y1_HPP

#include <boost/math/special_functions/bessel_j1.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <boost/assert.hpp>

// Bessel function of the second kind of order one

namespace boost { namespace math {

template <typename T>
T bessel_y1(T x)
{
    static const T P1[] = {
         4.0535726612579544093e+13L,
         5.4708611716525426053e+12L,
        -3.7595974497819597599e+11L,
         7.2144548214502560419e+09L,
        -5.9157479997408395984e+07L,
         2.2157953222280260820e+05L,
        -3.1714424660046133456e+02L,
    };
    static const T Q1[] = {
         3.0737873921079286084e+14L,
         4.1272286200406461981e+12L,
         2.7800352738690585613e+10L,
         1.2250435122182963220e+08L,
         3.8136470753052572164e+05L,
         8.2079908168393867438e+02L,
         1.0L,
    };
    static const T P2[] = {
         1.1514276357909013326e+19L,
        -5.6808094574724204577e+18L,
        -2.3638408497043134724e+16L,
         4.0686275289804744814e+15L,
        -5.9530713129741981618e+13L,
         3.7453673962438488783e+11L,
        -1.1957961912070617006e+09L,
         1.9153806858264202986e+06L,
        -1.2337180442012953128e+03L,
    };
    static const T Q2[] = {
         5.3321844313316185697e+20L,
         5.6968198822857178911e+18L,
         3.0837179548112881950e+16L,
         1.1187010065856971027e+14L,
         3.0221766852960403645e+11L,
         6.3550318087088919566e+08L,
         1.0453748201934079734e+06L,
         1.2855164849321609336e+03L,
         1.0L,
    };
    static const T PC[] = {
         2.2779090197304684302e+04L,
         4.1345386639580765797e+04L,
         2.1170523380864944322e+04L,
         3.4806486443249270347e+03L,
         1.5376201909008354296e+02L,
         8.8961548424210455236e-01L,
    };
    static const T QC[] = {
         2.2779090197304684318e+04L,
         4.1370412495510416640e+04L,
         2.1215350561880115730e+04L,
         3.5028735138235608207e+03L,
         1.5711159858080893649e+02L,
         1.0L,
    };
    static const T PS[] = {
        -8.9226600200800094098e+01L,
        -1.8591953644342993800e+02L,
        -1.1183429920482737611e+02L,
        -2.2300261666214198472e+01L,
        -1.2441026745835638459e+00L,
        -8.8033303048680751817e-03L,
    };
    static const T QS[] = {
         5.7105024128512061905e+03L,
         1.1951131543434613647e+04L,
         7.2642780169211018836e+03L,
         1.4887231232283756582e+03L,
         9.0593769594993125859e+01L,
         1.0L,
    };
    static const T x1  =  2.1971413260310170351e+0L,
                   x2  =  5.4296810407941351328e+0L,
                   x11 =  5.620e+02L,
                   x12 =  1.8288260310170351490E-03L,
                   x21 =  1.3900e+03L,
                   x22 = -6.4592058648672279948E-06L
    ;
    T value, factor, r, rc, rs;

    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_y1",
                        "domain error, argument must be non-negative");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            return -std::numeric_limits<T>::infinity();
        }
        else
        {
            domain_error<T>("boost::math::bessel_y1",
                            "infinity occurred but not supported");
        }
    }
    if (x <= 4.0)                       // x in (0, 4]
    {
        T y = x * x;
        T z = 2.0L * log(x/x1) * boost::math::bessel_j1(x) / pi<T>();
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        r = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        factor = (x + x1) * ((x - x11/256.0L) - x12) / x;
        value = z + factor * r;
    }
    else if (x <= 8.0)                  // x in (4, 8]
    {
        T y = x * x;
        T z = 2.0L * log(x/x2) * boost::math::bessel_j1(x) / pi<T>();
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = (x + x2) * ((x - x21/256.0L) - x22) / x;
        value = z + factor * r;
    }
    else                                // x \in (8, \infty)
    {
        T y = 8.0L / x;
        T y2 = y * y;
        T z = x - 0.75L * pi<T>();
        BOOST_ASSERT(sizeof(PC) == sizeof(QC));
        BOOST_ASSERT(sizeof(PS) == sizeof(QS));
        rc = evaluate_rational(PC, QC, y2, sizeof(PC)/sizeof(PC[0]));
        rs = evaluate_rational(PS, QS, y2, sizeof(PS)/sizeof(PS[0]));
        factor = sqrt(2.0L / (x * pi<T>()));
        value = factor * (rc * sin(z) + y * rs * cos(z));
    }
    
    return value;
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_Y1_HPP
