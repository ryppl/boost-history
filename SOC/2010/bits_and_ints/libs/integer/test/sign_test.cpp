//  Boost bit_reversal_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/bits_and_ints.hpp> 
#include <iostream> 


// Macros to compact code
#define SIGN_TEST(n, m) \
BOOST_TEST(::boost::sign(n) == m)

#define STATIC_SIGN_TEST(t, v, e) \
BOOST_TEST((::boost::static_sign<t, v>::value == e))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on sign functions." << std::endl;
	
	// Signed tests
	for (int i = -10000; i != 0; ++i) {
		SIGN_TEST(i, -1);
	}
	
	for (int i = 1; i != 10000; ++i) {
		SIGN_TEST(i, 1);
	}
	
	SIGN_TEST(0, 0);
	
	// Unsigned tests
	SIGN_TEST(unsigned(-1), 1);
	for (unsigned i = -1000; i != 40000; ++i) SIGN_TEST(i, (i != 0));
	
	SIGN_TEST(unsigned(0), 0);
	
	
	STATIC_SIGN_TEST(int, -1, -1);
	STATIC_SIGN_TEST(int, -10, -1);
	STATIC_SIGN_TEST(int, -123123, -1);
	STATIC_SIGN_TEST(int, 1, 1);
	STATIC_SIGN_TEST(int, 10, 1);
	STATIC_SIGN_TEST(int, 123123, 1);
	STATIC_SIGN_TEST(int, 0, 0);
	STATIC_SIGN_TEST(int, -1000, -1);
	STATIC_SIGN_TEST(int, -898989, -1);
	STATIC_SIGN_TEST(int, -7732, -1);
	STATIC_SIGN_TEST(int, 67763, 1);
	STATIC_SIGN_TEST(int, 112312, 1);
	STATIC_SIGN_TEST(int, 1123, 1);
	STATIC_SIGN_TEST(int, -564645, -1);
	STATIC_SIGN_TEST(int, -100000, -1);
	

#ifndef BOOST_NO_INT64_T
	
	STATIC_SIGN_TEST(int64_t, -10000000000, -1);
	STATIC_SIGN_TEST(int64_t, -20000000000, -1);
	STATIC_SIGN_TEST(int64_t, -1, -1);
	STATIC_SIGN_TEST(int64_t, 1, 1);
	STATIC_SIGN_TEST(int64_t, 10000000000, 1);
	STATIC_SIGN_TEST(int64_t, 12312300000, 1);
	STATIC_SIGN_TEST(int64_t, 0, 0);	
	
	// 64-bit tests
	for (int64_t i = -10000002000; i != -10000000000; ++i) {
		SIGN_TEST(i, -1);
	}
	
	for (int64_t i = 11000000000; i != 11000002000; ++i) {
		SIGN_TEST(i, 1);
	}
	
	SIGN_TEST(int64_t(0), 0);
#endif
	
	return boost::report_errors();
}

