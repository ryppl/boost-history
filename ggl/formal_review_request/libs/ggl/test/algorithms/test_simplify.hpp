// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_SIMPLIFY_HPP
#define GGL_TEST_SIMPLIFY_HPP

// Test-functionality, shared between single and multi tests

#include <ggl_test_common.hpp>
#include <ggl/algorithms/simplify.hpp>
#include <ggl/algorithms/distance.hpp>
#include <ggl/strategies/strategies.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

template <typename Tag, typename Geometry>
struct test_inserter
{
    static void apply(Geometry& , std::string const& , double )
    {}
};

template <typename Geometry>
struct test_inserter<ggl::linestring_tag, Geometry>
{
    static void apply(Geometry& geometry, std::string const& expected, double distance)
    {
        Geometry simplified;
        ggl::simplify_inserter(geometry,
            std::back_inserter(simplified), distance);

        std::ostringstream out;
        out << ggl::wkt(simplified);
        BOOST_CHECK_EQUAL(out.str(), expected);
    }
};


template <typename Geometry>
void test_geometry(std::string const& wkt, std::string const& expected, double distance)
{
    Geometry geometry, simplified;

    // Generate polygon using only integer coordinates and obvious results
    // Polygon is a hexagon, having one extra point (2,1) on a line which should be filtered out.
    ggl::read_wkt(wkt, geometry);
    ggl::simplify(geometry, simplified, distance);

    {
        std::ostringstream out;
        out << ggl::wkt(simplified);

        BOOST_CHECK_MESSAGE(out.str() == expected,
            "simplify: " << ggl::wkt(geometry)
            << " expected " << expected
            << " got " << ggl::wkt(simplified));
    }

    // Check using user-specified strategy
    typedef typename ggl::point_type<Geometry>::type point_type;
    typedef typename ggl::cs_tag<point_type>::type tag;
    typedef ggl::strategy::distance::xy_point_segment
        <
            point_type,
            point_type
        > strategy;
    typedef ggl::strategy::simplify::douglas_peucker
        <
            point_type,
            strategy
        > simplify_strategy_type;

    BOOST_CONCEPT_ASSERT( (ggl::concept::SimplifyStrategy<simplify_strategy_type>) );
    ggl::simplify(geometry, simplified, distance, simplify_strategy_type());

    {
        std::ostringstream out;
        out << ggl::wkt(simplified);
        BOOST_CHECK_EQUAL(out.str(), expected);
    }

    // Check inserter (if applicable)
    test_inserter
        <
            typename ggl::tag<Geometry>::type,
            Geometry
        >::apply(geometry, expected, distance);
}


#endif
