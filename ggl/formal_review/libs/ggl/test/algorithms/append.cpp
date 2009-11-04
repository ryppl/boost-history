// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <deque>
#include <vector>

#include <boost/concept/requires.hpp>
#include <ggl_test_common.hpp>

#include <ggl/core/access.hpp>
#include <ggl/algorithms/make.hpp>
#include <ggl/algorithms/clear.hpp>
#include <ggl/algorithms/append.hpp>
#include <ggl/algorithms/num_points.hpp>
#include <ggl/geometries/geometries.hpp>
#include <ggl/geometries/adapted/std_as_linestring.hpp>
#include <ggl/geometries/adapted/boost_array_as_linestring.hpp>

#include <test_common/test_point.hpp>

template <typename G>
void test_geometry()
{
    G geometry;
    typedef typename ggl::point_type<G>::type P;

    ggl::append(geometry, ggl::make_zero<P>());
    BOOST_CHECK_EQUAL(ggl::num_points(geometry), 1);

    ggl::clear(geometry);
    BOOST_CHECK_EQUAL(ggl::num_points(geometry), 0);
    //P p = boost::range::front(geometry);
}

template <typename P>
void test_all()
{
    test_geometry<ggl::linestring<P> >();
    test_geometry<ggl::linear_ring<P> >();
    test_geometry<ggl::polygon<P> >();

    test_geometry<std::vector<P> >();
    test_geometry<std::deque<P> >();
    //test_geometry<std::list<P> >();
}

int test_main(int, char* [])
{
    test_all<test::test_point>();
    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
