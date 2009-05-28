// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <iostream>
#include <iomanip>
#include <string>
#include <boost/numeric_adaptor/numeric_adaptor.hpp>
#include <boost/numeric_adaptor/ieee_policy.hpp>

#if defined(HAVE_GMP)
#  include <boost/numeric_adaptor/gmp_policy.hpp>
#endif

#if defined(HAVE_CLN)
#  include <boost/numeric_adaptor/cln_policy.hpp>
#endif



template <typename T, typename Num>
void sample1(std::string const& header, T const& t_a, T const& t_b, T const& t_c)
{
    std::cout << std::endl << "---" << header << std::endl;
    Num a = t_a;
    Num b = t_b;
    Num a_plus_b = a + b;

    //T t3 = a_plus_b; // does NOT compile for strings
    std::cout << "a + b:     " << T(a_plus_b) << std::endl;

    Num c = t_c;
    Num par = (a + b) * c;
    std::cout << "(a + b) c: " << T(par) << std::endl;
    par = a + b * c;
    std::cout << "a + bc:    " << T(par) << std::endl;

    Num sqrt_a_plus_b = Num::sqrt(a_plus_b);
    std::cout << "sqrt(a+b): " << T(sqrt_a_plus_b) << std::endl;

    // Calc the hypot function
    Num hypot_b_c = Num::hypot(b, c);
    std::cout << "hypot:     " << T(hypot_b_c) << std::endl;
    std::cout << "hypot2:    " << T(Num::sqrt(b * b + c * c)) << std::endl;
}




template <typename T, typename Num>
void sample2(
{
    std::cout << std::endl << "---" << header << std::endl;

    // Conversion from int
    Num n = 4;
    std::cout << T(n);

    // Assignment from T
    Num t = c1;
    n = t;
    std::cout << "  " << T(n) << std::endl;

    Num v1 = c1;
    Num v2 = c2;
    if (v1 > v2)
    {
        std::cout << "v1 > v2" << std::endl;
    }
    if (v1 < v2)
    {
        std::cout << "v1 < v2" << std::endl;
    }
    if (v1 == v2)
    {
        std::cout << "v1 == v2" << std::endl;
    }

    Num v7 = Num::cos(v1);
    std::cout << "cos: " << T(v7) << std::endl;
    v7 = Num::abs(v7);
    std::cout << "abs: " << T(v7) << std::endl;
    v7 = Num::sin(v1);
    std::cout << "sin: " << T(v7) << std::endl;

}

void long_double_issue()
{
    long double ld = 2.0000000002;
    std::cout << "Strange: " << sizeof(long double) << " " << double(ld) << std::endl;
    double d = 2.0000000002;
    std::cout << "OK: " << sizeof(double) << " " << d << std::endl;
}


int main()
{

    using namespace boost::numeric_adaptor;

    mpf_set_default_prec(128);
    std::cout << std::setprecision(18);

    long_double_issue();

    sample1<std::string, numeric_adaptor<gmp_policy> >("use string, calculate with gmp",
        "2.000000000002", "3.000000000003", "4.00000000004");
    sample1<std::string, numeric_adaptor<cln_policy> >("use string, calculate with cln",
        "2.000000000002", "3.000000000003", "4.00000000004");
    sample1<double, numeric_adaptor<ieee_policy<long double> > >("use double, calculate with ieee/long double",
        2.0000000000002,   3.000000000003,   4.00000000004);

    double a = 2.0000000002;
    double b = 3.0000000003;
    double c = 4.0000000004;



    sample2<float, numeric_adaptor<ieee_policy<float> > >("use float, calculate with float", a, b, c);
    sample2<float, numeric_adaptor<ieee_policy<double> > >("use float, calculate with double", a, b, c);
    sample2<double, numeric_adaptor<ieee_policy<double> > >("use double, calculate with double", a, b, c);


#if ! defined(_MSC_VER)
    sample2<double, numeric_adaptor<gmp_policy> >("use double, calculate with gmp", a, b, c);
    sample2<double, numeric_adaptor<cln_policy> >("use double, calculate with CLN", a, b, c);
#endif


    {
        double a = -3;
        double b = -4;
        std::cout << hypot(a, b) << std::endl;
        std::cout << boost::math::hypot(a, b) << std::endl;

        double rat = a / b;
        double h = b * sqrt(1 + rat*rat);
        std::cout << h << std::endl;

    }

    return 0;
}
