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
#include <boost/test/floating_point_comparison.hpp>
#include <geometry/strategies/strategies_point_xy.hpp>
#include "common.hpp"


using namespace geometry;


template <typename P1, typename P2>
void test_null_distance()
{
	P1 p1;
	init_point(p1, 1, 2, 3);
	P2 p2;
	init_point(p2, 1, 2, 3);
	BOOST_CHECK((geometry::strategy::distance::pythagoras<P1, P2>()(p1, p2) == 0));
}

template <typename P1, typename P2>
void test_axis()
{
	P1 p1;
	init_point(p1, 0, 0, 0);

	P2 p2;
	init_point(p2, 1, 0, 0);
	BOOST_CHECK((geometry::strategy::distance::pythagoras<P1, P2>()(p1, p2) == 1));
	init_point(p2, 0, 1, 0);
	BOOST_CHECK((geometry::strategy::distance::pythagoras<P1, P2>()(p1, p2) == 1));
	init_point(p2, 0, 0, 1);
	BOOST_CHECK((geometry::strategy::distance::pythagoras<P1, P2>()(p1, p2) == 1));
}

template <typename P1, typename P2>
void test_arbitrary()
{
	P1 p1;
	init_point(p1, 1, 2, 3);
	P2 p2;
	init_point(p2, 9, 8, 7);
	BOOST_CHECK_CLOSE((double)(geometry::strategy::distance::pythagoras<P1, P2>()(p1, p2)),
                          sqrt(116),
                          0.001
        );
}


template <typename P1, typename P2>
void test_all()
{
	test_null_distance<P1, P2>();
	test_axis<P1, P2>();
	test_arbitrary<P1, P2>();
}

template <typename P>
void test_all()
{
	test_all<P, int[3]>();
	test_all<P, float[3]>();
	test_all<P, double[3]>();
	test_all<P, test_point>();
}


BOOST_AUTO_TEST_CASE(pythagoras_test)
{
	test_all<int[3]>();
	test_all<float[3]>();
	test_all<double[3]>();
	test_all<test_point>();
}
