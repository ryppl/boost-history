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
#include "numeric_adaptor.hpp"
#include "ieee_policy.hpp"
#include "gmp_policy.hpp"


template <typename T, typename NUM>
void sample(const std::string& header, T c1, T c2, T c4, T ta, T tb, T tc)
{
	std::cout << std::endl << "---" << header << std::endl;
	NUM v1 = c1;
	NUM v2 = c2;
	NUM v3 = v1 + v2;
	std::cout << T(v3) << std::endl;

	NUM v4 = c4;
	NUM v5 = (v1 + v2) * v4;
	std::cout << T(v5) << std::endl;
	v5 = v1 + v2 * v4;
	std::cout << T(v5) << std::endl;

	NUM v6 = NUM::sqrt(v3);
	std::cout << T(v6) << std::endl;

	v6 = 4;
	std::cout << T(v6) << std::endl;

	v6 = v2;
	std::cout << T(v6) << std::endl;
	std::cout << std::endl;

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

	// Test Heron formule
	{
		NUM a = ta;
		NUM b = tb;
		NUM c = tc;
		NUM s((a + b + c) / NUM(2.0));
		NUM area = NUM::sqrt(s * (s - a) * (s - b) * (s - c));
		std::cout << "area: " << T(area) << std::endl;
	}

}


int main()
{
	std::cout << std::setprecision(12);
	double a = 31622.77662;
	double b = 0.000023;
	double c = 31622.77661;

	sample<float, numeric_adaptor<ieee_policy<float> > >("use float, calculate with float", 2.0, 3.0, 5.0, a, b, c);
	sample<float, numeric_adaptor<ieee_policy<double> > >("use float, calculate with double", 2.0, 3.0, 5.0, a, b, c);
	sample<double, numeric_adaptor<ieee_policy<long double> > >("use double, calculate with long double", 2.0, 3.0, 5.0, a, b, c);
	sample<double, numeric_adaptor<gmp_policy> >("use double, calculate with gmp", 2.0, 3.0, 5.0, a, b, c);
	return 0;
}
