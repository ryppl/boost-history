// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_INTERSECTS_HPP
#define GGL_TEST_INTERSECTS_HPP


#include <ggl_test_common.hpp>

#include <ggl/core/ring_type.hpp>
#include <ggl/algorithms/intersects.hpp>
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

    bool detected = ggl::intersects(geometry1, geometry2);

    BOOST_CHECK_MESSAGE(detected == expected,
        "intersects: " << wkt1
        << " with " << wkt2
        << " -> Expected: " << expected
        << " detected: " << detected);
}


template <typename Geometry>
void test_self_intersects(std::string const& wkt, bool expected)
{
    Geometry geometry;

    ggl::read_wkt(wkt, geometry);

    bool detected = ggl::intersects(geometry);

    BOOST_CHECK_MESSAGE(detected == expected,
        "intersects: " << wkt
        << " -> Expected: " << expected
        << " detected: " << detected);
}



#endif
