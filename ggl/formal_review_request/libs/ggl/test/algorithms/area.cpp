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
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/polygon.hpp>

//#define GGL_TEST_DEBUG

#include <algorithms/test_area.hpp>



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

template <typename Point>
void test_spherical()
{
    ggl::polygon<Point> geometry;

    // unit-sphere has area of 4-PI. Polygon covering 1/8 of it:
    double expected = 4.0 * ggl::math::pi / 8.0;
    ggl::read_wkt("POLYGON((0 0,0 90,90 0,0 0))", geometry);

    double area = ggl::area(geometry);
    BOOST_CHECK_CLOSE(area, expected, 0.0001);

    // With strategy, radius 2 -> 4 pi r^2 
    ggl::strategy::area::huiller
        <
            typename ggl::point_type<Point>::type
        > strategy(2.0); 

    area = ggl::area(geometry, strategy);
    BOOST_CHECK_CLOSE(area, 2.0 * 2.0 * expected, 0.0001);
}


int test_main(int, char* [])
{
    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    test_spherical<ggl::point<double, 2, ggl::cs::spherical<ggl::degree> > >();

#if defined(HAVE_CLN)
    test_all<ggl::point_xy<boost::numeric_adaptor::cln_value_type> >();
#endif
#if defined(HAVE_GMP)
    test_all<ggl::point_xy<boost::numeric_adaptor::gmp_value_type> >();
#endif

    return 0;
}
