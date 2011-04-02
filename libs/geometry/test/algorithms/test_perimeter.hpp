// Boost.Geometry (aka GGL, Generic Geometry Library) test file
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_TEST_PERIMETER_HPP
#define BOOST_GEOMETRY_TEST_PERIMETER_HPP


#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/perimeter.hpp>
#include <boost/geometry/strategies/strategies.hpp>


#include <boost/geometry/domains/gis/io/wkt/read_wkt.hpp>


template <typename Geometry>
void test_perimeter(Geometry const& geometry, long double expected_perimeter)
{
    typename bg::default_distance_result<Geometry>::type perimeter = bg::perimeter(geometry);

#ifdef GEOMETRY_TEST_DEBUG
    std::ostringstream out;
    out << typeid(typename bg::coordinate_type<Geometry>::type).name()
        << std::endl
        << typeid(typename bg::perimeter_result<Geometry>::type).name()
        << std::endl
        << "perimeter : " << bg::perimeter(geometry)
        << std::endl;
    std::cout << out.str();
#endif

    BOOST_CHECK_CLOSE(perimeter, expected_perimeter, 0.0001);
}


template <typename Geometry>
void test_geometry(std::string const& wkt, double expected_perimeter)
{
    Geometry geometry;
    bg::read_wkt(wkt, geometry);
    test_perimeter(geometry, expected_perimeter);
}


#endif
