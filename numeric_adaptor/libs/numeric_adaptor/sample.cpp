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

#if ! defined(_MSC_VER)
#  include <boost/numeric_adaptor/gmp_policy.hpp>
#  include <boost/numeric_adaptor/cln_policy.hpp>
#endif



template <typename T, typename Num>
void sample1(const std::string& header, T const& c1, T const& c2, T const& c4)
{
    std::cout << std::endl << "---" << header << std::endl;
    Num v1 = c1;
    Num v2 = c2;
    Num v3 = v1 + v2;

    // NOTE: strings can NOT be casted / streamed in one line for GCC/Comeau (for MSVC/DMC it is OK)
    // See http://www.codeguru.com/forum/showthread.php?p=1842725
    T t3 = v3;
    std::cout << "a + b: " << t3 << std::endl;

    Num v4 = c4;
    Num v5 = (v1 + v2) * v4;
    T t5 = v5;
    std::cout << "  " << t5;
    v5 = v1 + v2 * v4;
    T t5a = v5;
    std::cout << "  " << t5a;

    Num v6 = Num::sqrt(v3);
    T t6 = v6;
    std::cout << "  " << t6 << std::endl;

}




template <typename T, typename Num>
void sample2(const std::string& header, T const& c1, T const& c2, T const& c4)
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
    v7 = Num::sin(v1);
    std::cout << "sin: " << T(v7) << std::endl;

}

void long_double_issue()
{
    long double ld = 2.0000000002;
    std::cout << "Strange: " << sizeof(long double) << " " << ld << std::endl;
    double d = 2.0000000002;
    std::cout << "OK: " << sizeof(double) << " " << d << std::endl;
}


int main()
{
    mpf_set_default_prec(128);
    std::cout << std::setprecision(12);

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


    return 0;
}
