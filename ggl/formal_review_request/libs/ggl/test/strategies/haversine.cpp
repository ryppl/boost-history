// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>

#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>

#include <ggl/algorithms/assign.hpp>
#include <ggl/strategies/spherical/distance_haversine.hpp>
#include <ggl/strategies/concepts/distance_concept.hpp>


#include <ggl/geometries/point.hpp>


template <typename Point>
struct test_distance
{
    typedef ggl::strategy::distance::haversine
        <
            Point,
            Point
        > haversine_type;


    BOOST_CONCEPT_ASSERT
        (
            (ggl::concept::PointDistanceStrategy<haversine_type>)
        );


    static void test(double lon1, double lat1, double lon2, double lat2, 
                       double radius, double expected, double tolerance)
    {
        haversine_type strategy(radius);

        Point p1, p2;
        ggl::assign(p1, lon1, lat1);
        ggl::assign(p2, lon2, lat2);
        typename haversine_type::return_type d1 = strategy(p1, p2);

        BOOST_CHECK_CLOSE((double) d1, expected, tolerance);
    }
};

template <typename Point>
void test_all()
{
    double const average_earth_radius = 6372795.0;

    // earth to unit-sphere -> divide by earth circumference, then it is from 0-1,
    // then multiply with 2 PI, so effectively just divide by earth raius
    double e2u = 1.0 / average_earth_radius;

    // ~ Amsterdam/Paris
    double const a_p = 467.2704 * 1000.0;
    test_distance<Point>::test(4, 52, 2, 48, average_earth_radius, a_p, 1.0);
    test_distance<Point>::test(2, 48, 4, 52, average_earth_radius, a_p, 1.0);
    test_distance<Point>::test(4, 52, 2, 48, 1.0, a_p * e2u, 0.001);

    // ~ Amsterdam/Barcelona
    double const a_b = 1232.9065 * 1000.0;
    test_distance<Point>::test(4, 52, 2, 41, average_earth_radius, a_b, 1.0);
    test_distance<Point>::test(2, 41, 4, 52, average_earth_radius, a_b, 1.0);
    test_distance<Point>::test(4, 52, 2, 41, 1.0, a_b * e2u, 0.001);
}


int test_main(int, char* [])
{
    test_all<ggl::point<double, 2, ggl::cs::spherical<ggl::degree> > >();

    return 0;
}
