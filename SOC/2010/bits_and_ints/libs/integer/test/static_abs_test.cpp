//  Boost static_abs_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/static_abs.hpp> 

#include <iostream>  

// Macros to compact code
#define STATIC_ABS_TEST(t, v, e) \
BOOST_TEST((::boost::static_abs<t, v>::value == e))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on static_abs." << std::endl;
	
	STATIC_ABS_TEST(int, -1, 1);
	STATIC_ABS_TEST(int, 1, 1);
	STATIC_ABS_TEST(int, -100, 100);
	STATIC_ABS_TEST(int, 100, 100);
	STATIC_ABS_TEST(int, -999999, 999999);
	STATIC_ABS_TEST(int, 999999, 999999);
	STATIC_ABS_TEST(int, -7, 7);

	return boost::report_errors();
}

