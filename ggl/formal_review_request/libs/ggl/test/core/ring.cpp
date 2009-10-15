// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/test/included/test_exec_monitor.hpp>

// To be tested:
#include <ggl/core/ring_type.hpp>
#include <ggl/core/exterior_ring.hpp>
#include <ggl/core/interior_rings.hpp>

// For geometries:
#include <ggl/core/cs.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/polygon.hpp>


#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>




template <typename P>
void test_ring(std::string const& wkt,
    int expected_main_count,
    int expected_interior_ring_count,
    int expected_first_interior_count)
{
    typedef ggl::polygon<P> the_polygon;
    typedef typename ggl::ring_type<the_polygon>::type the_ring;
    typedef typename ggl::interior_type<the_polygon>::type the_interior;

    the_polygon poly;
    read_wkt(wkt, poly);

    the_ring ext = ggl::exterior_ring(poly);
    the_interior rings = ggl::interior_rings(poly);

    BOOST_CHECK_EQUAL(ggl::num_interior_rings(poly), expected_interior_ring_count);
    BOOST_CHECK_EQUAL(boost::size(rings), expected_interior_ring_count);
    BOOST_CHECK_EQUAL(boost::size(ext), expected_main_count);
    if (boost::size(rings) > 0)
    {
        BOOST_CHECK_EQUAL(boost::size(rings.front()), expected_first_interior_count);
    }
}


template <typename P>
void test_all()
{
    test_ring<P>("POLYGON((0 0,0 3,3 3,3 0,0 0),(1 1,1 2,2 2,2 1,1 1))", 5, 1, 5);
    test_ring<P>("POLYGON((0 0,0 3,3 3,3 0,0 0),(1 1,2 2,2 1,1 1),(1 1,1 2,2 2,1 1))", 5, 2, 4);
    test_ring<P>("POLYGON((0 0,0 3,3 3,3 0,0 0))", 5, 0, 0);
}


int test_main(int, char* [])
{
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();
    return 0;
}
