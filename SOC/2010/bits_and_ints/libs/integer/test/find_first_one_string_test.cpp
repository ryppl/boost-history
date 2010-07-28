//  Boost integer/find_first_one_string.hpp test program  -------//
//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <iostream>
#include <cmath>
#include <limits>
#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/find_first_one_string.hpp>

int main()
{
	using boost::find_first_one_string;
	
	BOOST_TEST((find_first_one_string((uint32_t)0, 1) == -1));
	BOOST_TEST((find_first_one_string((uint32_t)0, 2) == -1));
	BOOST_TEST((find_first_one_string((uint32_t)0, 3) == -1));
	BOOST_TEST((find_first_one_string((uint32_t)0, 30) == -1));
	BOOST_TEST((find_first_one_string((uint32_t)0, 31) == -1));
	BOOST_TEST((find_first_one_string((uint32_t)0x0F, 4) == 28));
	BOOST_TEST((find_first_one_string((uint32_t)0x1F, 4) == 27));
	BOOST_TEST((find_first_one_string((uint32_t)0xFFFFFFFF, 1) == 0));
	BOOST_TEST((find_first_one_string((uint32_t)0xFFFFFFFF, 10) == 0));
	BOOST_TEST((find_first_one_string((uint32_t)0xFFFFFFFF, 31) == 0));
	BOOST_TEST((find_first_one_string((uint32_t)0xFFFFFFFF, 32) == 0));
	BOOST_TEST((find_first_one_string((uint32_t)0x0000FFFF, 16) == 16));
	BOOST_TEST((find_first_one_string((uint32_t)0x0000FFFF, 20) == -1));
	
#ifndef BOOST_HAS_NO_INT64_T
	BOOST_TEST((find_first_one_string((uint64_t)0ULL, 1) == -1));
	BOOST_TEST((find_first_one_string((uint64_t)0ULL, 20) == -1));
	BOOST_TEST((find_first_one_string((uint64_t)0xFFFF0000FFFFULL, 16) == 16));
	BOOST_TEST((find_first_one_string((uint64_t)0xFFFF0000FF0FFFFFULL, 17) == 44));
	BOOST_TEST((find_first_one_string((uint64_t)0xFFFFFFFFFFFFFFFFULL, 64) == 0));
	BOOST_TEST((find_first_one_string((uint64_t)0xFFFFFFFFFFFFFFFFULL, 63) == 0));
#endif
	
	return boost::report_errors();
}
