//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_kv.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_kv(T, const char* type_name)
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
        SC_(2.4), SC_(1e-10), SC_(3.278104553186023132746733625747991686993e+24),
        SC_(5.5), SC_(3.1416), SC_(1.306237091469712177684916479081418832134),
        SC_(-5.5), SC_(10), SC_(7.330453007985021646448368795774845330962e-05),
        SC_(-5.5), SC_(1e+02), SC_(5.412745553067922673220844486939577479244e-45),
        SC_(-10.0003), SC_(1e-02), SC_(1.862153491634353444615450979718986806508e+28),
        SC_(10.0003), SC_(5e+01), SC_(9.151528521947647292475441287635760091206e-23),
        SC_(1.414e+02), SC_(1e+02), SC_(1.396588080387589351583405335851463349596e-06),
        SC_(1.414e+02), SC_(2e+02), SC_(9.123016600070846788538826824853321325972e-68),
        SC_(-1.414e+02), SC_(5e+01), SC_(1.303195111895922597045420580628979002549e+42),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data, type_name, "boost::math::bessel_kv", boost::math::bessel_kv);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_kv(0.0F, "float");
    test_bessel_kv(0.0, "double");
    test_bessel_kv(0.0L, "long double");

    return 0;
}
