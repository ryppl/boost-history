//  Boost isign_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/isign.hpp>
#include <boost/integer/static_isign.hpp>
#include <boost/mpl/integral_c.hpp>

#include <iostream> 

#define ISIGN_TEST(x, y, z) \
BOOST_TEST((::boost::isign(x, y) == z)); \
BOOST_TEST(((::boost::mpl::isign< ::boost::mpl::integral_c<int, x>, ::boost::mpl::integral_c<int, y> >::value) == z))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on safe_avg functions." << std::endl;
	
	ISIGN_TEST(0, 0, 0);
	ISIGN_TEST(0, 1, 0);
	ISIGN_TEST(0, -1, 0);
	ISIGN_TEST(1, 1, 1);
	ISIGN_TEST(1, -1, -1);
	ISIGN_TEST(-1, 1, 1);
	ISIGN_TEST(-1, -1, -1);
	ISIGN_TEST(123987, 1000, 123987);
	ISIGN_TEST(-123987, 1000, 123987);
	ISIGN_TEST(123987, -1000, -123987);
	ISIGN_TEST(-123987, -1000, -123987);
	ISIGN_TEST(9883230, -7632945, -9883230);
	ISIGN_TEST(-7632945, 9883230, 7632945);
	ISIGN_TEST(7632945, -9883230, -7632945);
	
	return boost::report_errors();
}
