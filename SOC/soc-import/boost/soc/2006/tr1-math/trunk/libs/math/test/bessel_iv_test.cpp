//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_iv.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_iv(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data = {
        SC_(-2.4), SC_(0), std::numeric_limits<T>::infinity(),
        SC_(2.4), SC_(1e-10), SC_(6.355298617027088084774423284374870973686e-26),
        SC_(5.5), SC_(3.1416), SC_(6.030637989611760876719525528587576047721e-02),
        SC_(-5.5), SC_(10), SC_(5.975776069613691696079374198699267057303e+02),
        SC_(-5.5), SC_(1e+02), SC_(9.223629061447068717373540691338138193587e+41),
        SC_(-10.0003), SC_(1e-02), SC_(1.117291929572154744246636013609443579168e+25),
        SC_(10.0003), SC_(5e+01), SC_(1.071532654574974277106092803988359713856e+20),
        SC_(1.414e+02), SC_(1e+02), SC_(2.067204118185020354450156053758015776929e+03),
        SC_(1.414e+02), SC_(2e+02), SC_(2.237573822812855890470540653631360684910e+64),
        SC_(-1.414e+02), SC_(1e+02), SC_(2.067204117339440203939536756493854073650e+03),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data, type_name, "boost::math::bessel_iv", boost::math::bessel_iv);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_iv(0.0F, "float");
    test_bessel_iv(0.0, "double");
    test_bessel_iv(0.0L, "long double");

    return 0;
}
