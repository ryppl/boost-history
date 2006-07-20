//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_jn.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_jn(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data = {
        SC_(2), SC_(0), SC_(0),
        SC_(2), SC_(1e-02), SC_(1.249989583365885362413250958437642113452e-05),
        SC_(5), SC_(10), SC_(-0.2340615281867936404436949416457777864635),
        SC_(-5), SC_(1e+06), SC_(7.259643842453285052375779970433848914846e-04),
        SC_(-5), SC_(-1), SC_(2.497577302112344313750655409880451981584e-04),
        SC_(10), SC_(10), SC_(0.2074861066333588576972787235187534280327),
        SC_(10), SC_(-5), SC_(1.467802647310474131107532232606627020895e-03),
        SC_(-10), SC_(1e+06), SC_(-3.310793117604488741264958559035744460210e-04),
        SC_(1e+02), SC_(8e+01), SC_(4.606553064823477354141298259169874909670e-06),
        SC_(1e+03), SC_(1e+05), SC_(1.283178112502480365195139312635384057363e-03),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data, type_name, "boost::math::bessel_jn", boost::math::bessel_jn);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_jn(0.0F, "float");
    test_bessel_jn(0.0, "double");
    test_bessel_jn(0.0L, "long double");

    return 0;
}
