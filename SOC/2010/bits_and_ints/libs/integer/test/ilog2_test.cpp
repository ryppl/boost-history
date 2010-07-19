//  Boost ilog2_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/ilog2.hpp>
#include <boost/cstdint.hpp>
#include <iostream>

#define ILOG2_TEST(x, y) \
BOOST_TEST((::boost::ilog2(x) == y)); \
std::cout << "deu " << ::boost::ilog2(x) << " era pra ser " << y << std::endl

// Main testing function
int main(int, char* [])
{    
	using namespace boost;
	std::cout << "Doing tests on ilog2 function." << std::endl;
	
	ILOG2_TEST((unsigned)100, 6);
	ILOG2_TEST((unsigned)265, 8);
	ILOG2_TEST((unsigned)158741, 17);
	ILOG2_TEST((unsigned)85256985, 26);
	ILOG2_TEST((unsigned)1073741824, 30);
	ILOG2_TEST((unsigned)8, 3);
	ILOG2_TEST((unsigned)21, 4);
	ILOG2_TEST((unsigned)23, 4);
	ILOG2_TEST((unsigned)1, 0);
	
#ifndef BOOST_HAS_NO_INT64_T
	ILOG2_TEST(8589934592ULL, 33);
	ILOG2_TEST(17179869181ULL, 33);
	ILOG2_TEST(34359738369ULL, 35);
#endif
	return boost::report_errors();
}