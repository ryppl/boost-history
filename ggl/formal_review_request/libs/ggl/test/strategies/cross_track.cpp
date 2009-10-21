// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>

#include <ggl/algorithms/assign.hpp>

#include <ggl/strategies/spherical/distance_haversine.hpp>
#include <ggl/strategies/spherical/distance_cross_track.hpp>

#include <ggl/strategies/concepts/distance_concept.hpp>

#include <ggl/geometries/point.hpp>
#include <ggl/geometries/segment.hpp>


template <typename Point>
void test_distance(double lon1, double lat1, 
                   double lon2, double lat2, 
                   double lon3, double lat3,
                   double radius, double expected, double tolerance)
{
    typedef ggl::strategy::distance::cross_track
        <
            Point,
            ggl::segment<const Point>
        > strategy_type;


    BOOST_CONCEPT_ASSERT
        (
            (ggl::concept::PointSegmentDistanceStrategy<strategy_type>)
        );
    

    strategy_type strategy(radius);

    Point p1, p2, p3;
    ggl::assign(p1, lon1, lat1);
    ggl::assign(p2, lon2, lat2);
    ggl::assign(p3, lon3, lat3);
    typename strategy_type::return_type d 
        = strategy(p1, ggl::segment<const Point>(p2, p3));

    BOOST_CHECK_CLOSE((double) d, expected, tolerance);
}

template <typename Point>
void test_all()
{
    double const average_earth_radius = 6372795.0;

    // distance (Paris <-> Amsterdam/Barcelona), ~87 km
    // should be is equal 
    // to distance (Paris <-> Barcelona/Amsterdam)
    double const p_to_ab = 86.8 * 1000.0;
    test_distance<Point>(2, 48, 4, 52, 2, 41, average_earth_radius, p_to_ab, 1.0);
    test_distance<Point>(2, 48, 2, 41, 4, 52, average_earth_radius, p_to_ab, 1.0);
}

int test_main(int, char* [])
{
    test_all<ggl::point<double, 2, ggl::cs::spherical<ggl::degree> > >();

    return 0;
}
