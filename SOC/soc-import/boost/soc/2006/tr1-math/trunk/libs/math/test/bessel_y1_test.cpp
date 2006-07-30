//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_y1.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_y1(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 2>, 10> data = {
        SC_(0), -std::numeric_limits<T>::infinity(),
        SC_(1), SC_(-0.7812128213002887165471500000479648205499),
        SC_(2), SC_(-0.1070324315409375468883707722774766366875),
        SC_(4), SC_(0.3979257105571000052539799724507918522712),
        SC_(8), SC_(-0.1580604617312474942555552661874835503273),
        SC_(1e-10), SC_(-6.366197723675813431507891842844626117091e+09),
        SC_(1e-20), SC_(-6.366197723675813779877633701282183024017e+19),
        SC_(1e+01), SC_(0.2490154242069538839232834746632228032604),
        SC_(1e+03), SC_(-2.478433129235177891486235609714129093863e-02),
        SC_(1e+05), SC_(1.719210350088256300994945235398971029545e-03),
    };
    #undef SC_

    boost::math::tools::test_univariate
        (data, type_name, "boost::math::bessel_y1", boost::math::bessel_y1);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_y1(0.0F, "float");
    test_bessel_y1(0.0, "double");
    test_bessel_y1(0.0L, "long double");

    return 0;
}
