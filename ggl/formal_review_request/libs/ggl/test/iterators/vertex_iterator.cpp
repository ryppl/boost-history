// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <deque>

#include <ggl_test_common.hpp>

#include <ggl/iterators/vertex_iterator.hpp>

#include <ggl/core/cs.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/linestring.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/polygon.hpp>

template <typename G, bool IsConst, typename Expected>
void test_geometry()
{
    typedef typename ggl::vertex_iterator<G, IsConst>::type it;
    BOOST_CHECK_EQUAL(typeid(it).name(), typeid(Expected).name());
}

template <typename P>
void test_all()
{
    test_geometry<ggl::linestring<P>, true,
                typename std::vector<P>::const_iterator>();
    test_geometry<ggl::linestring<P, std::deque>, true,
                typename std::deque<P>::const_iterator>();

    test_geometry<ggl::linestring<P>, false,
                typename std::vector<P>::iterator>();

    test_geometry<ggl::linear_ring<P>, true,
                typename std::vector<P>::const_iterator>();
    test_geometry<std::vector<P>, true,
                typename std::vector<P>::const_iterator>();

    // So by nature of point_const_iterator, even a polygon should
    // deliver a the iterator of the rings making it up
    test_geometry<ggl::polygon<P>, true,
                typename std::vector<P>::const_iterator>();
    test_geometry<ggl::polygon<P, std::deque>, true,
                typename std::deque<P>::const_iterator>();
    test_geometry<ggl::polygon<P, std::deque, std::deque>, true,
                typename std::deque<P>::const_iterator>();
    test_geometry<ggl::polygon<P, std::deque, std::deque>, false,
                typename std::deque<P>::iterator>();
}

int test_main(int, char* [])
{
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
