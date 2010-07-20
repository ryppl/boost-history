//  Boost count_trailing_zeros_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/count_trailing_zeros.hpp>
#include <boost/integer/static_count_trailing_zeros.hpp>

#define COUNT_ZEROS_TEST(x, y) \
BOOST_TEST((::boost::count_trailing_zeros(unsigned(x)) == y)); \
BOOST_TEST(((::boost::mpl::count_trailing_zeros< ::boost::mpl::integral_c< ::boost::uintmax_t, x> >::value) == y))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on count_trailing_zeros functions." << std::endl;
	
	COUNT_ZEROS_TEST(0xF00000000, 32);
	COUNT_ZEROS_TEST(0x1, 0);
	COUNT_ZEROS_TEST(0x2, 1);
	COUNT_ZEROS_TEST(0x4, 2);
	COUNT_ZEROS_TEST(0x8, 3);
	COUNT_ZEROS_TEST(0x3, 0);
	COUNT_ZEROS_TEST(0x5, 0);
	COUNT_ZEROS_TEST(0x9, 0);
	COUNT_ZEROS_TEST(0x90, 4);
	COUNT_ZEROS_TEST(0xAA, 1);
	COUNT_ZEROS_TEST(0xFFFF, 0);
	COUNT_ZEROS_TEST(0xF0A0, 5);
	COUNT_ZEROS_TEST(0xFFFFFFFF, 0);
	COUNT_ZEROS_TEST(0x55555555, 0);
	COUNT_ZEROS_TEST(0xBABEBEEF, 0);
	COUNT_ZEROS_TEST(0xFF800, 11);
	COUNT_ZEROS_TEST(0x800, 11);
	COUNT_ZEROS_TEST(0x123800, 11);
	COUNT_ZEROS_TEST(0x33800, 11);
	COUNT_ZEROS_TEST(0x80000000, 31);
	
	return boost::report_errors();
}