// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_WITHIN_HPP
#define GGL_TEST_WITHIN_HPP


#include <ggl_test_common.hpp>

#include <ggl/core/ring_type.hpp>
#include <ggl/algorithms/within.hpp>
#include <ggl/strategies/strategies.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/polygon.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>


template <typename Geometry1, typename Geometry2>
void test_geometry(std::string const& wkt1,
        std::string const& wkt2, bool expected)
{
    Geometry1 geometry1;
    Geometry2 geometry2;

    ggl::read_wkt(wkt1, geometry1);
    ggl::read_wkt(wkt2, geometry2);

    bool detected = ggl::within(geometry1, geometry2);

    BOOST_CHECK_MESSAGE(detected == expected,
        "within: " << wkt1
        << " in " << wkt2
        << " -> Expected: " << expected
        << " detected: " << detected);
}


template <typename Point, bool CW>
void test_ordered_ring(std::string const& wkt_point,
        std::string const& wkt_geometry, bool expected, bool on_border)
{
    typedef ggl::linear_ring<Point, std::vector, CW> ring_type;
    ring_type ring;
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

    // other strategy (note that this one cannot detect OnBorder
    // (without modifications)

    ggl::strategy::within::franklin<Point> franklin;
    detected = ggl::within(point, ring, franklin);
    if (! on_border)
    {
        BOOST_CHECK_MESSAGE(detected == expected,
            "within: " << wkt_point
            << " in " << wkt_geometry
            << " -> Expected: " << expected
            << " detected: " << detected);
    }


    ggl::strategy::within::crossings_multiply<Point> cm;
    detected = ggl::within(point, ring, cm);
    if (! on_border)
    {
        BOOST_CHECK_MESSAGE(detected == expected,
            "within: " << wkt_point
            << " in " << wkt_geometry
            << " -> Expected: " << expected
            << " detected: " << detected);
    }
}

template <typename Point>
void test_ring(std::string const& wkt_point,
        std::string const& wkt_geometry,
        bool expected, bool on_border)
{
    test_ordered_ring<Point, true>(wkt_point, wkt_geometry, expected, on_border);
    test_ordered_ring<Point, false>(wkt_point, wkt_geometry, expected, on_border);
    test_geometry<Point, ggl::polygon<Point> >(wkt_point, wkt_geometry, expected);
}

#endif
