//  Boost count_leading_zeros_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/count_leading_zeros.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/cstdint.hpp>
#include <iostream>

#define CLZ_TEST(x, y) \
BOOST_TEST((::boost::count_leading_zeros(x) == y))

// Main testing function
int main(int, char* [])
{    
	using namespace boost;
	std::cout << "Doing tests on safe_avg functions." << std::endl;
	
	CLZ_TEST(uint8_t(0xFF), 0);
	CLZ_TEST(uint8_t(0x7F), 1);
	CLZ_TEST(uint8_t(0x3F), 2);
	CLZ_TEST(uint8_t(0x1F), 3);
	CLZ_TEST(uint8_t(0x0F), 4);
	CLZ_TEST(uint8_t(0x07), 5);
	CLZ_TEST(uint8_t(0x03), 6);
	CLZ_TEST(uint8_t(0x01), 7);
	CLZ_TEST(uint8_t(0x00), 8);
	
	CLZ_TEST(uint16_t(0xF000), 0);
	CLZ_TEST(uint16_t(0xAA00), 0);
	CLZ_TEST(uint16_t(0x0000), 16);
	CLZ_TEST(uint16_t(0x0002), 14);
	
	CLZ_TEST(uint32_t(0x0), 32);
	CLZ_TEST(uint32_t(0xFFFFFFFF), 0);
	CLZ_TEST(uint32_t(0x0001FFFF), 15);
	CLZ_TEST(uint32_t(0x00FFFFFF), 8);
	CLZ_TEST(uint32_t(0x007FFFFF), 9);
	
#ifndef BOOST_HAS_NO_INT64_T
	
	CLZ_TEST(0x0LL, 64);
	CLZ_TEST(0xFLL, 60);
	CLZ_TEST(0xFFFFFFFFLL, 32);
	CLZ_TEST(0x7FFFFFFFLL, 33);
	CLZ_TEST(0xFFFFFFFFFFFFFFFFLL, 0);
	
#endif
	return boost::report_errors();
}