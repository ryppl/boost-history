//  Boost integer/static_abs.hpp test program  ------------------//
//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/static_lcm.hpp>
#include <boost/mpl/integral_c.hpp>

#include <iostream>  


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on mpl::gcd." << std::endl;
	
	using boost::mpl::integral_c;
	using boost::mpl::lcm;
	
	typedef integral_c<int, 0> zero;
	typedef integral_c<int, 1> one;
	typedef integral_c<int, 3> three;
	typedef integral_c<int, 6> six;
	typedef integral_c<int, 7> seven;
	typedef integral_c<int, 8> eight;
	typedef integral_c<int, 9> nine;
	typedef integral_c<int, 18> eighteen;
	typedef integral_c<int, 30> thirty;
	typedef integral_c<int, 49> fortynine;
	
	BOOST_TEST((lcm<one, one>::value) ==  1);
    BOOST_TEST((lcm<zero, zero>::value) ==  0);
    BOOST_TEST((lcm<six, zero>::value) ==  0);
    BOOST_TEST((lcm<zero, seven>::value) ==  0);
    BOOST_TEST((lcm<eighteen, thirty>::value) == 90);
    BOOST_TEST((lcm<three, seven>::value) == 21);
    BOOST_TEST((lcm<eight, nine>::value) == 72);
    BOOST_TEST((lcm<seven, fortynine>::value) == 49);
	
	return boost::report_errors();
}


