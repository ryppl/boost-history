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
void sample(const std::string& header, T c1, T c2, T c4)
{
    std::cout << std::endl << "---" << header << std::endl;
    Num v1 = c1;
    Num v2 = c2;
    Num v3 = v1 + v2;
    std::cout << T(v3);

    Num v4 = c4;
    Num v5 = (v1 + v2) * v4;
    std::cout << "  " << T(v5);
    v5 = v1 + v2 * v4;
    std::cout << "  " << T(v5);

    Num v6 = Num::sqrt(v3);
    std::cout << "  " << T(v6) << std::endl;

    // Conversion from int
    v6 = 4;
    std::cout << T(v6);

    // Assignment from T
    v6 = v2;
    std::cout << "  " << T(v6) << std::endl;

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

    Num v7 = Num::cos(v3);
    std::cout << "cos: " << T(v7) << std::endl;
    v7 = Num::sin(v3);
    std::cout << "sin: " << T(v7) << std::endl;

}


int main()
{
    std::cout << std::setprecision(12);

    double a = 2.000000000002;
    double b = 3.000000000003;
    double c = 5.000000000005;

    sample<float, numeric_adaptor<ieee_policy<float> > >("use float, calculate with float", a, b, c);
    sample<float, numeric_adaptor<ieee_policy<double> > >("use float, calculate with double", a, b, c);

    sample<double, numeric_adaptor<ieee_policy<double> > >("use double, calculate with double", a, b, c);

#if ! defined(_MSC_VER)
    sample<double, numeric_adaptor<gmp_policy> >("use double, calculate with gmp", a, b, c);
    sample<double, numeric_adaptor<cln_policy> >("use double, calculate with CLN", a, b, c);
#endif

    sample<std::string, numeric_adaptor<ieee_policy<double> > >("use string, calculate with gmp","2.000000000002", "3.000000000003", "5.000000000005");

    return 0;
}
