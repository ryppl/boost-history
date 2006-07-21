//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_k1.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_k1(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 2>, 10> data = {
        SC_(0), std::numeric_limits<T>::infinity(),
        SC_(1), SC_(0.6019072301972345747375400015356173392616),
        SC_(2), SC_(0.1398658818165224272845988070354110238872),
        SC_(4), SC_(1.248349888726843147038417998080606848384e-02),
        SC_(8), SC_(1.553692118050011339168624506224746211171e-04),
        SC_(1e-05), SC_(9.99999999393557150960518670942038610458e+04),
        SC_(5e-10), SC_(1.999999999999999994491913866708807771225e+09),
        SC_(1e-20), SC_(1.000000000000000054839999999999999999998e+20),
        SC_(5e+01), SC_(3.444102226717555612591853035912671550997e-23),
        SC_(1e+02), SC_(4.679853735636909286562544242024335307975e-45),
    };
    #undef SC_

    boost::math::tools::test_univariate
        (data, type_name, "boost::math::bessel_k1", boost::math::bessel_k1);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_k1(0.0F, "float");
    test_bessel_k1(0.0, "double");
    test_bessel_k1(0.0L, "long double");

    return 0;
}
