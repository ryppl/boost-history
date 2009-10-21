// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/within.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>

#include <ggl/algorithms/assign.hpp>

#include <ggl/geometries/box.hpp>
#include <ggl/geometries/cartesian2d.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/linestring.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/point_xy.hpp>
#include <ggl/geometries/polygon.hpp>


template <typename Point, typename Geometry>
void test_geometry(std::string const& wkt_point,
        std::string const& wkt_geometry, bool expected)
{
    Geometry geometry;
    Point point;

    ggl::read_wkt(wkt_geometry, geometry);
    ggl::read_wkt(wkt_point, point);

    bool detected = ggl::within(point, geometry);

    BOOST_CHECK_MESSAGE(detected == expected,
        "within: " << wkt_point
        << " in " << wkt_geometry
        << " -> Expected: " << expected
        << " detected: " << detected);
}

template <typename Geometry>
void test_circle(std::string const& wkt_geometry, bool expected)
{
    ggl::circle circle;
    ggl::assign(circle, 1.0, 1.0, 3.0);

    Geometry geometry;
    ggl::read_wkt(wkt_geometry, geometry);

    bool detected = ggl::within(geometry, circle);

    BOOST_CHECK_MESSAGE(detected == expected,
        "within: " << wkt_geometry
        << " in circle (1,1) with radius 3"
        << " -> Expected: " << expected
        << " detected: " << detected);
}

template <typename Point, bool CW>
void test_ordered_ring(std::string const& wkt_point,
        std::string const& wkt_geometry, bool expected)
{
    ggl::linear_ring<Point, std::vector, CW> ring;
    Point point;

    ggl::read_wkt(wkt_geometry, ring);
    if (! CW)
    {
        std::reverse(boost::begin(ring), boost::end(ring));
    }
    ggl::read_wkt(wkt_point, point);

    bool detected = ggl::within(point, ring);

    BOOST_CHECK_MESSAGE(detected == expected,
        "within: " << wkt_point
        << " in " << wkt_geometry
        << " -> Expected: " << expected
        << " detected: " << detected);
}

template <typename Point>
void test_ring(std::string const& wkt_point,
        std::string const& wkt_geometry, bool expected)
{
    test_ordered_ring<Point, true>(wkt_point, wkt_geometry, expected);
    test_ordered_ring<Point, false>(wkt_point, wkt_geometry, expected);
    test_geometry<Point, ggl::polygon<Point> >(wkt_point, wkt_geometry, expected);
}


template <typename P>
void test_all()
{
    // trivial case
    test_ring<P>("POINT(1 1)", "POLYGON((0 0,0 2,2 2,2 0,0 0))", true);

    // on border/corner
    test_ring<P>("POINT(0 0)", "POLYGON((0 0,0 2,2 2,2 0,0 0))", false);
    test_ring<P>("POINT(0 1)", "POLYGON((0 0,0 2,2 2,2 0,0 0))", false);

    // aligned to segment/vertex
    test_ring<P>("POINT(1 1)", "POLYGON((0 0,0 3,3 3,3 1,2 1,2 0,0 0))", true);
    test_ring<P>("POINT(1 1)", "POLYGON((0 0,0 3,4 3,3 1,2 2,2 0,0 0))", true);

    // same polygon, but point outside
    test_ring<P>("POINT(3 3)", "POLYGON((0 0,0 3,3 3,3 1,2 1,2 0,0 0))", false);
    test_ring<P>("POINT(3 3)", "POLYGON((0 0,0 3,4 3,3 1,2 2,2 0,0 0))", false);

    // holes
    test_geometry<P, ggl::polygon<P> >("POINT(2 2)",
        "POLYGON((0 0,0 4,4 4,4 0,0 0),(1 1,3 1,3 3,1 3,1 1))", false);


    test_geometry<P, ggl::box<P> >("POINT(1 1)", "BOX(0 0,2 2)", true);

    // Real-life problem (solved now), point is in the middle, 409623 is also a coordinate
    // on the border, it was wrong first.
    test_ring<P>("POINT(146383 409623)",
        "POLYGON((146351 410597,146521 410659,147906 410363,148088 410420"
        ",148175 410296,148281 409750,148215 409623,148154 409666,148154 409666"
        ",148130 409625,148035 409626,148035 409626,148008 409544,147963 409510"
        ",147993 409457,147961 409352,147261 408687,147008 408586,145714 408840"
        ",145001 409033,144486 409066,144616 409308,145023 410286,145254 410488"
        ",145618 410612,145618 410612,146015 410565,146190 410545,146351 410597))",
        true);
}


template <typename P>
void test_circles()
{
    // Circles do not yet run on CLN/GMP
    test_circle<P>("POINT(2 1)", true);
    test_circle<P>("POINT(12 1)", false);

    test_circle<ggl::linestring<P> >("LINESTRING(1 1,2 1,2 2)", true);
    test_circle<ggl::linestring<P> >("LINESTRING(1 1,2 1,2 2,10 10)", false);
}


int test_main( int , char* [] )
{
    //test_all<ggl::point_xy<int> >();
    test_all<ggl::point_xy<double> >();
    test_circles<ggl::point_xy<double> >();

#if defined(HAVE_CLN)
    test_all<ggl::point_xy<boost::numeric_adaptor::cln_value_type> >();
#endif
#if defined(HAVE_GMP)
    test_all<ggl::point_xy<boost::numeric_adaptor::gmp_value_type> >();
#endif

    return 0;
}
