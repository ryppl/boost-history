//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_yn.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_yn(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data = {
        SC_(2), SC_(0), -std::numeric_limits<T>::infinity(),
        SC_(2), SC_(1e-20), SC_(-1.273239544735162825799983373532762142498e+40),
        SC_(5), SC_(10), SC_(0.1354030476893623031970290147622417090884),
        SC_(-5), SC_(1e+06), SC_(3.310520883226090485035355700146889670969e-04),
        SC_(10), SC_(10), SC_(-0.3598141521834027220519865773435606093584),
        SC_(10), SC_(1e-10), SC_(-1.182804904943349339009609377195656698776e+108),
        SC_(-10), SC_(1e+06), SC_(7.259519692951870862452513663653936536109e-04),
        SC_(1e+02), SC_(5), SC_(-5.084863916020222879930915630930820355951e+115),
        SC_(1e+03), SC_(1e+05), SC_(2.172549191376840370928341466292126477646e-03),
        SC_(-1e+03), SC_(7e+02), SC_(-1.887531099809458899608438032843452617962e+77),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data, type_name, "boost::math::bessel_yn", boost::math::bessel_yn);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_yn(0.0F, "float");
    test_bessel_yn(0.0, "double");
    test_bessel_yn(0.0L, "long double");

    return 0;
}
