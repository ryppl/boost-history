//  Boost integer/rotate.hpp test program  ----------------------//
//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/rotate.hpp>

#define ROTATE_L_TEST(x, n, y) \
BOOST_TEST((::boost::rotate_left(x, n) == y))

#define ROTATE_R_TEST(x, n, y) \
BOOST_TEST((::boost::rotate_right(x, n) == y))

int main()
{
    ROTATE_L_TEST(0x0, 1, 0);
	ROTATE_L_TEST(0x0, 30, 0);
	ROTATE_L_TEST(0xF0000000, 4, 0xF);
	ROTATE_L_TEST(0xFF000000, 4, 0xF000000F);
	ROTATE_L_TEST(0xFF000000, 8, 0xFF);
	ROTATE_L_TEST(0xA0000000, 1, 0x40000001);
	ROTATE_L_TEST(0xA0000000, 2, 0x80000002);
	ROTATE_L_TEST(0xA0000000, 3, 0x5);
	ROTATE_L_TEST(0xC, 8, 0xC00);
	ROTATE_L_TEST(0xFFFFFFFF, 4, 0xFFFFFFFF);
	ROTATE_L_TEST(0xFFFFFFFF, 31, 0xFFFFFFFF);
	ROTATE_L_TEST(0x00000002, 31, 0x1);
	ROTATE_L_TEST(0x12345678, 4, 0x23456781);
	
#ifndef BOOST_HAS_NO_INT64_T
	
    ROTATE_L_TEST((uint64_t)0x0, 1, 0);
	ROTATE_L_TEST((uint64_t)0x0, 30, 0);
	ROTATE_L_TEST((uint64_t)0xF000000000000000, 4, 0xF);
	ROTATE_L_TEST((uint64_t)0xFF00000000000000, 4, 0xF00000000000000F);
	ROTATE_L_TEST((uint64_t)0xFF00000000000000, 8, 0xFF);
	ROTATE_L_TEST((uint64_t)0xA000000000000000, 1, 0x4000000000000001);
	ROTATE_L_TEST((int64_t)0xA000000000000000, 2, (int64_t)0x8000000000000002);
	ROTATE_L_TEST((int64_t)0xA000000000000000, 3, (int64_t)0x5);
	ROTATE_L_TEST((int64_t)0xC, 8, (int64_t)0xC00);
	ROTATE_L_TEST((int64_t)0xFFFFFFFFFFFFFFFF, 4, (int64_t)0xFFFFFFFFFFFFFFFF);
	ROTATE_L_TEST((int64_t)0xFFFFFFFFFFFFFFFF, 63, (int64_t)0xFFFFFFFFFFFFFFFF);
	ROTATE_L_TEST((int64_t)0x0000000000000002, 63, (int64_t)0x1);
	ROTATE_L_TEST((int64_t)0x123456789ABCDEF0, 4, (int64_t)0x23456789ABCDEF01);

#endif
	
	return boost::report_errors();
}
