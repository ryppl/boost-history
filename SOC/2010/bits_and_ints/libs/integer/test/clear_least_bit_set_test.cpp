//  Boost clear_least_bit_set_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/clear_least_bit_set.hpp>
#include <boost/integer/static_clear_least_bit_set.hpp>
#include <boost/mpl/integral_c.hpp>

#include <iostream> 

#define CLEAR_LEAST_BIT_SET_TEST(x, y) \
BOOST_TEST((::boost::clear_least_bit_set(x) == y)); \
BOOST_TEST(((::boost::mpl::clear_least_bit_set< ::boost::mpl::integral_c<int, x> >::value) == y))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on clear_least_bit_set functions." << std::endl;
	
	CLEAR_LEAST_BIT_SET_TEST(0x0, 0);
	CLEAR_LEAST_BIT_SET_TEST(0x1, 0);
	CLEAR_LEAST_BIT_SET_TEST(0x2, 0);
	CLEAR_LEAST_BIT_SET_TEST(0x4, 0);
	CLEAR_LEAST_BIT_SET_TEST(0x8, 0);
	CLEAR_LEAST_BIT_SET_TEST(0x3, 2);
	CLEAR_LEAST_BIT_SET_TEST(0x5, 4);
	CLEAR_LEAST_BIT_SET_TEST(0x9, 8);
	CLEAR_LEAST_BIT_SET_TEST(0x90, 0x80);
	CLEAR_LEAST_BIT_SET_TEST(0xAA, 0xA8);
	CLEAR_LEAST_BIT_SET_TEST(0xFFFF, 0xFFFE);
	CLEAR_LEAST_BIT_SET_TEST(0xF0A0, 0xF080);
	CLEAR_LEAST_BIT_SET_TEST(0xFFFFFFF, 0xFFFFFFE);
	CLEAR_LEAST_BIT_SET_TEST(0x55555555, 0x55555554);
	CLEAR_LEAST_BIT_SET_TEST(0x1ABEBEEF, 0x1ABEBEEE);
	CLEAR_LEAST_BIT_SET_TEST(0xFF800, 0xFF000);
	CLEAR_LEAST_BIT_SET_TEST(0x800, 0);
	CLEAR_LEAST_BIT_SET_TEST(0x123800, 0x123000);
	CLEAR_LEAST_BIT_SET_TEST(0x33800, 0x33000);
	CLEAR_LEAST_BIT_SET_TEST(0x42000000, 0x40000000);
	
	return boost::report_errors();
}