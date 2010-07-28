//  Boost integer/static_abs.hpp test program -------------------//
//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/static_gcd.hpp>
#include <boost/mpl/integral_c.hpp>

#include <iostream>  


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on mpl::gcd." << std::endl;
	
	using boost::mpl::integral_c;
	using boost::mpl::gcd;
	
	typedef integral_c<int, 0> zero;
	typedef integral_c<int, 1> one;
	typedef integral_c<int, 3> three;
	typedef integral_c<int, 7> seven;
	typedef integral_c<int, 8> eight;
	typedef integral_c<int, 9> nine;
	typedef integral_c<int, 30> thirty;
	typedef integral_c<int, 42> fortytwo;
	typedef integral_c<int, 49> fortynine;
	
	BOOST_TEST((gcd<one, one>::value == 1));
	BOOST_TEST((gcd<zero, zero>::value == 0));
	BOOST_TEST((gcd<seven, zero>::value == 7));
	BOOST_TEST((gcd<zero, nine>::value == 9));
	BOOST_TEST((gcd<fortytwo, thirty>::value == 6));
	BOOST_TEST((gcd<three, seven>::value == 1));
	BOOST_TEST((gcd<eight, nine>::value == 1));
	BOOST_TEST((gcd<seven, fortynine>::value == 7));
	
	return boost::report_errors();
}


