//  Boost integer/inc_rev.hpp test program  ---------------------//
//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/inc_rev.hpp>

#define INC_REV_TEST(a, b) \
BOOST_TEST(a == b)


int main()
{
	using boost::inc_rev;
	uint8_t c = 0;
	
	INC_REV_TEST((c = inc_rev(c)), 128);
	INC_REV_TEST((c = inc_rev(c)), 64);
	INC_REV_TEST((c = inc_rev(c)), 192);
	INC_REV_TEST((c = inc_rev(c)), 32);
	INC_REV_TEST((c = inc_rev(c)), 160);
	INC_REV_TEST((c = inc_rev(c)), 96);
	INC_REV_TEST((c = inc_rev(c)), 224);
	INC_REV_TEST((c = inc_rev(c)), 16);
	INC_REV_TEST((c = inc_rev(c)), 144);
	INC_REV_TEST((c = inc_rev(c)), 80);
	INC_REV_TEST((c = inc_rev(c)), 208);
	INC_REV_TEST((c = inc_rev(c)), 48);
	INC_REV_TEST((c = inc_rev(c)), 176);
	INC_REV_TEST((c = inc_rev(c)), 112);
	INC_REV_TEST((c = inc_rev(c)), 240);
	INC_REV_TEST((c = inc_rev(c)), 8);
	INC_REV_TEST((c = inc_rev(c)), 136);
	INC_REV_TEST((c = inc_rev(c)), 72);
	INC_REV_TEST((c = inc_rev(c)), 200);
	
	return boost::report_errors();
}