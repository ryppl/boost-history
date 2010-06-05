//  Boost same_sign_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/bits_and_ints.hpp> 

#include <iostream>  

// Macros to compact code
#define SAME_SIGN_TEST(n, m, r) \
BOOST_TEST(::boost::same_sign(n, m) == r)

#define STATIC_SAME_SIGN_TEST(t, n, m, r) \
BOOST_TEST((::boost::static_same_sign<t, n, m>::value == r))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on same_sign." << std::endl;
	
	for (int i = 0; i != 101; ++i) {
		for (int j = -100; j != 1; ++j) {
			SAME_SIGN_TEST(i, j, false);
			STATIC_SAME_SIGN_TEST(int, i, j, false);
		}
		for (int j = 0; j != 100; ++j) {
			SAME_SIGN_TEST(i, j, true);
			STATIC_SAME_SIGN_TEST(int, i, j, true);
		}
	}
	
	return boost::report_errors();
}

