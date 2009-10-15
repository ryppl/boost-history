// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept/assert.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <ggl/core/geometry_id.hpp>

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
    BOOST_CHECK_EQUAL(ggl::geometry_id<G>::type::value, Expected);
}

template <typename P>
void test_all()
{
    test_geometry<P, 1>();
    test_geometry<const P, 1>();
    test_geometry<ggl::linestring<P> , 2>();
    test_geometry<ggl::linear_ring<P> , 93>();
    test_geometry<ggl::polygon<P> , 3>();
    test_geometry<ggl::box<P> , 94>();
    test_geometry<ggl::segment<P> , 92>();
    test_geometry<ggl::segment<const P> , 92>();
    test_geometry<ggl::nsphere<P, double> , 91>();

    test_geometry<std::vector<P>, 2>();
    test_geometry<std::deque<P>, 2>();

    test_geometry<boost::array<P, 5>, 2>();
}

int test_main(int, char* [])
{
    test_geometry<int[2], 1>();
    test_geometry<float[2], 1>();
    test_geometry<double[2], 1>();

    test_geometry<int[3], 1>();
    test_geometry<float[3], 1>();
    test_geometry<double[3], 1>();

    test_geometry<boost::tuple<double, double>, 1>();
    test_geometry<boost::tuple<double, double, double>, 1>();

    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
