//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_Y1_HPP
#define BOOST_MATH_BESSEL_Y1_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/special_functions/bessel_j1.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <boost/assert.hpp>

// Bessel function of the second kind of order one
// x <= 8, minimax rational approximations on root-bracketing intervals
// x > 8, Hankel asymptotic expansion in Hart, Computer Approximations, 1968

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
        -4.4357578167941278571e+06L,
        -9.9422465050776411957e+06L,
        -6.6033732483649391093e+06L,
        -1.5235293511811373833e+06L,
        -1.0982405543459346727e+05L,
        -1.6116166443246101165e+03L,
         0.0L,
    };
    static const T QC[] = {
        -4.4357578167941278568e+06L,
        -9.9341243899345856590e+06L,
        -6.5853394797230870728e+06L,
        -1.5118095066341608816e+06L,
        -1.0726385991103820119e+05L,
        -1.4550094401904961825e+03L,
         1.0L,
    };
    static const T PS[] = {
         3.3220913409857223519e+04L,
         8.5145160675335701966e+04L,
         6.6178836581270835179e+04L,
         1.8494262873223866797e+04L,
         1.7063754290207680021e+03L,
         3.5265133846636032186e+01L,
         0.0L,
    };
    static const T QS[] = {
         7.0871281941028743574e+05L,
         1.8194580422439972989e+06L,
         1.4194606696037208929e+06L,
         4.0029443582266975117e+05L,
         3.7890229745772202641e+04L,
         8.6383677696049909675e+02L,
         1.0L,
    };
    static const T x1  =  2.1971413260310170351e+00L,
                   x2  =  5.4296810407941351328e+00L,
                   x11 =  5.620e+02L,
                   x12 =  1.8288260310170351490e-03L,
                   x21 =  1.3900e+03L,
                   x22 = -6.4592058648672279948e-06L
    ;
    T value, factor, r, rc, rs;

    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    if (x < 0)
    {
        return domain_error<T>("boost::math::bessel_y1(x)",
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
            overflow_error<T>("boost::math::bessel_y1(x)",
                              "infinity occurred but not supported");
        }
    }
    if (x <= 4.0)                       // x in (0, 4]
    {
        T y = x * x;
        T z = 2.0L * log(x/x1) * bessel_j1(x) / pi<T>();
        BOOST_ASSERT(sizeof(P1) == sizeof(Q1));
        r = evaluate_rational(P1, Q1, y, sizeof(P1)/sizeof(P1[0]));
        factor = (x + x1) * ((x - x11/256.0L) - x12) / x;
        value = z + factor * r;
    }
    else if (x <= 8.0)                  // x in (4, 8]
    {
        T y = x * x;
        T z = 2.0L * log(x/x2) * bessel_j1(x) / pi<T>();
        BOOST_ASSERT(sizeof(P2) == sizeof(Q2));
        r = evaluate_rational(P2, Q2, y, sizeof(P2)/sizeof(P2[0]));
        factor = (x + x2) * ((x - x21/256.0L) - x22) / x;
        value = z + factor * r;
    }
    else                                // x in (8, \infty)
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
