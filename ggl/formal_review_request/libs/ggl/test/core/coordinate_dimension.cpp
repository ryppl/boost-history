// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>


#include <ggl/core/coordinate_dimension.hpp>


#include <ggl/geometries/geometries.hpp>

#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>

#include <ggl/geometries/adapted/boost_array_as_linestring.hpp>
#include <ggl/geometries/adapted/std_as_linestring.hpp>

#include <vector>
#include <deque>


template <typename G, std::size_t Expected>
void test_geometry()
{
    BOOST_CHECK_EQUAL(ggl::dimension<G>::type::value, Expected);
}

template <typename P, size_t D>
void test_all()
{
    test_geometry<P, D>();
    test_geometry<const P, D>();
    test_geometry<ggl::linestring<P> , D>();
    test_geometry<ggl::linear_ring<P> , D>();
    test_geometry<ggl::polygon<P> , D>();
    test_geometry<ggl::box<P> , D>();
    test_geometry<ggl::segment<P> , D>();
    test_geometry<ggl::segment<const P> , D>();
    test_geometry<ggl::nsphere<P, double> , D>();

    test_geometry<std::vector<P>, D>();
    test_geometry<std::deque<P>, D>();

    test_geometry<boost::array<P, 5>, D>();
}

int test_main(int, char* [])
{
    test_geometry<int[2], 2>();
    test_geometry<float[2], 2>();
    test_geometry<double[2], 2>();

    test_geometry<int[3], 3>();
    test_geometry<float[3], 3>();
    test_geometry<double[3], 3>();

    test_geometry<boost::tuple<double, double>, 2>();
    test_geometry<boost::tuple<double, double, double>, 3>();

    test_all<ggl::point<int, 2, ggl::cs::cartesian>, 2 >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian>, 2 >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian>, 2 >();

    return 0;
}
