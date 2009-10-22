// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/area.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>

#include <ggl/geometries/point.hpp>
#include <ggl/geometries/box.hpp>
#include <ggl/geometries/nsphere.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/polygon.hpp>

#include <algorithms/test_area.hpp>



template <typename P, typename T>
void test_area_circle()
{
    ggl::nsphere<P, T> c;

    ggl::set<0>(c.center(), 0);
    ggl::set<1>(c.center(), 0);
    c.radius(2);

    double d = ggl::area(c);
    BOOST_CHECK_CLOSE(d, 4 * 3.1415926535897932384626433832795, 0.001);
}


template <typename P>
void test_all()
{
    //test_area_circle<P, double>();
    test_geometry<ggl::box<P> >("POLYGON((0 0,2 2))", 4.0);
    test_geometry<ggl::box<P> >("POLYGON((2 2,0 0))", 4.0);

    // clockwise rings (second is wrongly ordered)
    test_geometry<ggl::linear_ring<P> >("POLYGON((0 0,0 7,4 2,2 0,0 0))", 16.0);
    test_geometry<ggl::linear_ring<P> >("POLYGON((0 0,2 0,4 2,0 7,0 0))", -16.0);

    // counter clockwise rings (first is wrongly ordered)
    test_geometry<ggl::linear_ring<P, std::vector, false> >
            ("POLYGON((0 0,0 7,4 2,2 0,0 0))", -16.0);
    test_geometry<ggl::linear_ring<P, std::vector, false> >
            ("POLYGON((0 0,2 0,4 2,0 7,0 0))", 16.0);

    test_geometry<ggl::polygon<P> >("POLYGON((0 0,0 7,4 2,2 0,0 0))", 16.0);
    test_geometry<ggl::polygon<P> >("POLYGON((1 1,2 1,2 2,1 2,1 1))", -1.0);
    test_geometry<ggl::polygon<P> >
            ("POLYGON((0 0,0 7,4 2,2 0,0 0), (1 1,2 1,2 2,1 2,1 1))", 15.0);
    test_geometry<ggl::polygon<P, std::vector, std::vector, false> >
            ("POLYGON((0 0,0 7,4 2,2 0,0 0), (1 1,2 1,2 2,1 2,1 1))", -15.0);
}


int test_main(int, char* [])
{
    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
