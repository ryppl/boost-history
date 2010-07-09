//  Boost safe_avg_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/safe_avg.hpp>
#include <boost/integer/static_safe_avg.hpp>
#include <boost/mpl/integral_c.hpp>

#include <iostream> 

#define SAFE_AVG_TEST(x, y, z) \
BOOST_TEST((::boost::safe_avg(x, y) == z)); \
BOOST_TEST(((::boost::mpl::safe_avg< ::boost::mpl::integral_c<int, x>, ::boost::mpl::integral_c<int, y> >::value) == z))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on safe_avg functions." << std::endl;
	
	SAFE_AVG_TEST(0, 0, 0);
	SAFE_AVG_TEST(0, 1, 0);
	SAFE_AVG_TEST(1, 1, 1);
	SAFE_AVG_TEST(1, 2, 1);
	SAFE_AVG_TEST(3, 1, 2);
	SAFE_AVG_TEST(1000, 200, 600);
	SAFE_AVG_TEST(1000000000, 2000000000, 1500000000);
	SAFE_AVG_TEST(2000000000, 2000000000, 2000000000);
	SAFE_AVG_TEST(-1000000000, 2000000000, 500000000);
	SAFE_AVG_TEST(-2000000000, -2000000000, -2000000000);
	
	return boost::report_errors();
}
