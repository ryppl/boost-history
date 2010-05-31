//  Boost sign_extend.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <boost/detail/lightweight_test.hpp>

#include <boost/cstdlib.hpp>              // for boost::exit_success
#include <boost/integer/bits_and_ints.hpp>  // for boost::sign_extend and boost::static_sign_extend

#include <iostream>  // for std::cout (std::endl indirectly)


// Macros to compact code
#define SIGN_EXTEND_TEST(d, b, e) \
	BOOST_TEST(::boost::sign_extend(d, b) == e); \
	BOOST_TEST((::boost::static_sign_extend<intmax_t, d, b>::value) == e)


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on sign_extend." << std::endl;
	
	SIGN_EXTEND_TEST(0, 1, 0);
	SIGN_EXTEND_TEST(1, 1, -1);
	
	SIGN_EXTEND_TEST(0, 2, 0);
	SIGN_EXTEND_TEST(1, 2, 1);
	SIGN_EXTEND_TEST(2, 2, -2);
	SIGN_EXTEND_TEST(3, 2, -1);
	
	SIGN_EXTEND_TEST(0, 3, 0);
	SIGN_EXTEND_TEST(1, 3, 1);
	SIGN_EXTEND_TEST(2, 3, 2);
	SIGN_EXTEND_TEST(3, 3, 3);
	SIGN_EXTEND_TEST(4, 3, -4);
	SIGN_EXTEND_TEST(5, 3, -3);
	SIGN_EXTEND_TEST(6, 3, -2);
	SIGN_EXTEND_TEST(7, 3, -1);
	
	SIGN_EXTEND_TEST(0, 4, 0);
	SIGN_EXTEND_TEST(1, 4, 1);
	SIGN_EXTEND_TEST(2, 4, 2);
	SIGN_EXTEND_TEST(3, 4, 3);
	SIGN_EXTEND_TEST(4, 4, 4);
	SIGN_EXTEND_TEST(5, 4, 5);
	SIGN_EXTEND_TEST(6, 4, 6);
	SIGN_EXTEND_TEST(7, 4, 7);
	SIGN_EXTEND_TEST(8, 4, -8);
	SIGN_EXTEND_TEST(9, 4, -7);
	SIGN_EXTEND_TEST(10, 4, -6);
	SIGN_EXTEND_TEST(11, 4, -5);
	SIGN_EXTEND_TEST(12, 4, -4);
	SIGN_EXTEND_TEST(13, 4, -3);
	SIGN_EXTEND_TEST(14, 4, -2);
	SIGN_EXTEND_TEST(15, 4, -1);
	
	SIGN_EXTEND_TEST(0x40000000, 31, -0x40000000);
	SIGN_EXTEND_TEST(0x7FFFFFFF, 31, -1);
	
#ifndef BOOST_NO_INT64_T
	SIGN_EXTEND_TEST(0x7FFFFFFFFF, 39, 0xFFFFFFFFFFFFFFFF);
	SIGN_EXTEND_TEST(0x1000000000000000, 63, 0x1000000000000000);
	SIGN_EXTEND_TEST(0x7FFFFFFFFFFFFFFF, 63, (uint64_t)-0x1);
#endif
	
	return boost::report_errors();
}

