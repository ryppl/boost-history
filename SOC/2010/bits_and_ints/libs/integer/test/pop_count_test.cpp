//  Boost pop_count_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/pop_count.hpp>
#include <boost/integer/static_pop_count.hpp>
#include <boost/mpl/integral_c.hpp>

#include <iostream> 

#define POP_COUNT_TEST(x, y) \
BOOST_TEST((::boost::pop_count(x) == y)); \
BOOST_TEST(((::boost::mpl::pop_count< ::boost::mpl::integral_c<uintmax_t, x> >::value) == y))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on pop_count functions." << std::endl;
	
	POP_COUNT_TEST(0x0, 0);
	POP_COUNT_TEST(0x1, 1);
	POP_COUNT_TEST(0x2, 1);
	POP_COUNT_TEST(0x4, 1);
	POP_COUNT_TEST(0x8, 1);
	POP_COUNT_TEST(0x3, 2);
	POP_COUNT_TEST(0x5, 2);
	POP_COUNT_TEST(0x9, 2);
	POP_COUNT_TEST(0x90, 2);
	POP_COUNT_TEST(0xAA, 4);
	POP_COUNT_TEST(0xFFFF, 16);
	POP_COUNT_TEST(0xF0A0, 6);
	POP_COUNT_TEST(0xFFFFFFFF, 32);
	POP_COUNT_TEST(0x55555555, 16);
	POP_COUNT_TEST(0xBABEBEEF, 24);
	
	return boost::report_errors();
}