//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_Y0_HPP
#define BOOST_MATH_BESSEL_Y0_HPP
#endif

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/special_functions/bessel_j0.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <boost/assert.hpp>

// Bessel function of the second kind of order zero
// x <= 8, minimax rational approximations on root-bracketing intervals
// x > 8, asymptotic approximation in Hart, Computer Approximations, 1968

namespace boost { namespace math {

template <typename T>
T bessel_y0(T x)
{
    static const T P1[] = {
         1.0723538782003176831e+11L,
        -8.3716255451260504098e+09L,
         2.0422274357376619816e+08L,
        -2.1287548474401797963e+06L,
         1.0102532948020907590e+04L,
        -1.8402381979244993524e+01L,
    };
    static const T Q1[] = {
         5.8873865738997033405e+11L,
         8.1617187777290363573e+09L,
         5.5662956624278251596e+07L,
         2.3889393209447253406e+05L,
         6.6475986689240190091e+02L,
         1.0L,
    };
    static const T P2[] = {
        -2.2213976967566192242e+13L,
        -5.5107435206722644429e+11L,
         4.3600098638603061642e+10L,
        -6.9590439394619619534e+08L,
         4.6905288611678631510e+06L,
        -1.4566865832663635920e+04L,
         1.7427031242901594547e+01L,
    };
    static const T Q2[] = {
         4.3386146580707264428e+14L,
         5.4266824419412347550e+12L,
         3.4015103849971240096e+10L,
         1.3960202770986831075e+08L,
         4.0669982352539552018e+05L,
         8.3030857612070288823e+02L,
         1.0L,
    };
    static const T P3[] = {
        -8.0728726905150210443e+15L,
         6.7016641869173237784e+14L,
        -1.2829912364088687306e+11L,
        -1.9363051266772083678e+11L,
         2.1958827170518100757e+09L,
        -1.0085539923498211426e+07L,
         2.1363534169313901632e+04L,
        -1.7439661319197499338e+01L,
    };
    static const T Q3[] = {
         3.4563724628846457519e+17L,
         3.9272425569640309819e+15L,
         2.2598377924042897629e+13L,
         8.6926121104209825246e+10L,
         2.4727219475672302327e+08L,
         5.3924739209768057030e+05L,
         8.7903362168128450017e+02L,
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
    static const T x1  =  8.9357696627916752158e-01L,
                   x2  =  3.9576784193148578684e+00L,
                   x3  =  7.0860510603017726976e+00L,
                   x11 =  2.280e+02L,
                   x12 =  2.9519662791675215849e-03L,
                   x21 =  1.0130e+03L,
                   x22 =  6.4716931485786837568e-04L,
                   x31 =  1.8140e+03L,
                   x32 =  1.1356030177269762362e-04L
    ;
    T value, factor, r, rc, rs;

    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_y0(x)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            return -std::numeric_limits<T>::infinity();
        }
        else
        {
            overflow_error<T>("boost::math::bessel_y0(x)",
                              "infinity occurred but not supported");
        }
    }
    if (x <= 3.0)                       // x in (0, 3]
    {
        T y = x * x;
        T z = 2.0L * log(x/x1) * bessel_j0(x) / pi<T>();
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        r = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        factor = (x + x1) * ((x - x11/256.0L) - x12);
        value = z + factor * r;
    }
    else if (x <= 5.0)                  // x in (3, 5]
    {
        T y = x * x;
        T z = 2.0L * log(x/x2) * bessel_j0(x) / pi<T>();
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = (x + x2) * ((x - x21/256.0L) - x22);
        value = z + factor * r;
    }
    else if (x <= 8.0)                  // x in (5, 8]
    {
        T y = x * x;
        T z = 2.0L * log(x/x3) * bessel_j0(x) / pi<T>();
        BOOST_ASSERT(sizeof(P3) == sizeof(Q3));
        r = evaluate_rational(P3, Q3, y, sizeof(P3)/sizeof(P3[0]));
        factor = (x + x3) * ((x - x31/256.0L) - x32);
        value = z + factor * r;
    }
    else                                // x in (8, \infty)
    {
        T y = 8.0L / x;
        T y2 = y * y;
        T z = x - 0.25L * pi<T>();
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
