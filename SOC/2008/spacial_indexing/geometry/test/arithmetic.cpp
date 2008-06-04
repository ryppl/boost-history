// Geometry Library test file
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#define BOOST_TEST_MODULE
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <geometry/arithmetic.hpp>
#include "common.hpp"


using namespace geometry;


template <typename P>
void test_addition()
{
	P p1;
	init_point(p1, 1, 2, 3);
	geometry::add_value(p1, 10);
	BOOST_CHECK(get<0>(p1) == 11);
	BOOST_CHECK(get<1>(p1) == 12);
	BOOST_CHECK(get<2>(p1) == 13);

	P p2;
	init_point(p2, 4, 5, 6);
	geometry::add_point(p1, p2);
	BOOST_CHECK(get<0>(p1) == 15);
	BOOST_CHECK(get<1>(p1) == 17);
	BOOST_CHECK(get<2>(p1) == 19);
}

template <typename P>
void test_subtraction()
{
	P p1;
	init_point(p1, 1, 2, 3);
	geometry::subtract_value(p1, 10);
	BOOST_CHECK(get<0>(p1) == -9);
	BOOST_CHECK(get<1>(p1) == -8);
	BOOST_CHECK(get<2>(p1) == -7);

	P p2;
	init_point(p2, 4, 6, 8);
	geometry::subtract_point(p1, p2);
	BOOST_CHECK(get<0>(p1) == -13);
	BOOST_CHECK(get<1>(p1) == -14);
	BOOST_CHECK(get<2>(p1) == -15);
}

template <typename P>
void test_multiplication()
{
	P p;
	init_point(p, 1, 2, 3);
	geometry::multiply(p, 5);
	BOOST_CHECK(get<0>(p) == 5);
	BOOST_CHECK(get<1>(p) == 10);
	BOOST_CHECK(get<2>(p) == 15);
}

template <typename P>
void test_division()
{
	P p;
	init_point(p, 50, 100, 150);
	geometry::divide(p, 5);
	BOOST_CHECK(get<0>(p) == 10);
	BOOST_CHECK(get<1>(p) == 20);
	BOOST_CHECK(get<2>(p) == 30);
}


template <typename P>
void test_all()
{
	test_addition<P>();
	test_subtraction<P>();
	test_multiplication<P>();
	test_division<P>();
}


BOOST_AUTO_TEST_CASE(arithmetic_test)
{
	test_all<int[3]>();
	test_all<float[3]>();
	test_all<double[3]>();
	test_all<test_point>();
}
