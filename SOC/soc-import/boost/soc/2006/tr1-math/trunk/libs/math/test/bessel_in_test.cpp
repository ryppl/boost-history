//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_in.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_in(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data = {
        SC_(-2), SC_(0), SC_(0),
        SC_(2), SC_(1e-10), SC_(1.250000000000000000001041666666666666667e-21),
        SC_(5), SC_(10), SC_(7.771882864032599599072934848023396328527e+02),
        SC_(-5), SC_(1e+02), SC_(9.470093873035581246182755550021617423216e+41),
        SC_(-5), SC_(-1), SC_(-2.714631559569718751810739051537773423836e-04),
        SC_(10), SC_(20), SC_(3.540200209019521099052891382449856070573e+06),
        SC_(10), SC_(-5), SC_(4.580044419176051261186470278720169531923e-03),
        SC_(1e+02), SC_(9), SC_(2.743066017460589970935876546689590715229e-93),
        SC_(1e+02), SC_(8e+01), SC_(4.651948328506102053181281914041458850940e+08),
        SC_(-1e+02), SC_(-2e+02), SC_(4.352750449727021914387290174411982575082e+74),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data, type_name, "boost::math::bessel_in", boost::math::bessel_in);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_in(0.0F, "float");
    test_bessel_in(0.0, "double");
    test_bessel_in(0.0L, "long double");

    return 0;
}
