// Geometry Library test file
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_TEST_COMMON_HPP
#define _GEOMETRY_TEST_COMMON_HPP


#include <boost/tuple/tuple.hpp>
#include <geometry/concepts/point_traits.hpp>


// Test point class

struct test_point: public boost::tuple<float, float, float>
{
	typedef float coordinate_type;
	enum { coordinate_count = 3 };
};


// Generic initialization function

template <typename P>
void init_point(P& p,
          typename geometry::point_traits<P>::coordinate_type c1,
          typename geometry::point_traits<P>::coordinate_type c2,
          typename geometry::point_traits<P>::coordinate_type c3)
{
	geometry::get<0>(p) = c1;
	geometry::get<1>(p) = c2;
	geometry::get<2>(p) = c3;
}


#endif
