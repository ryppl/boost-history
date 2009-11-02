// Generic Geometry2 Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_OVERLAPS_HPP
#define GGL_TEST_OVERLAPS_HPP


#include <ggl_test_common.hpp>

#include <ggl/core/ring_type.hpp>
#include <ggl/algorithms/overlaps.hpp>
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

    bool detected = ggl::overlaps(geometry1, geometry2);

    BOOST_CHECK_MESSAGE(detected == expected,
        "overlaps: " << wkt1
        << " in " << wkt2
        << " -> Expected: " << expected
        << " detected: " << detected);
}


#endif
