// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_LENGTH_HPP
#define GGL_TEST_LENGTH_HPP

#include <ggl_test_common.hpp>

#include <ggl/algorithms/length.hpp>
#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>
#include <ggl/strategies/strategies.hpp>


template <typename Geometry>
void test_length(Geometry const& geometry, long double expected_length)
{
    long double length = ggl::length(geometry);

#ifdef GGL_TEST_DEBUG
    std::ostringstream out;
    out << typeid(typename ggl::coordinate_type<Geometry>::type).name() 
        << std::endl
        << typeid(typename ggl::length_result<Geometry>::type).name() 
        << std::endl
        << "length : " << ggl::length(geometry) 
        << std::endl;
    std::cout << out.str();
#endif

    BOOST_CHECK_CLOSE(length, expected_length, 0.0001);
}


template <typename Geometry>
void test_geometry(std::string const& wkt, double expected_length)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);
    test_length(geometry, expected_length);
}


#endif
