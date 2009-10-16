// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>


#include <ggl/core/tag.hpp>

#include <ggl/geometries/geometries.hpp>

#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>

#include <ggl/geometries/adapted/boost_array_as_linestring.hpp>
#include <ggl/geometries/adapted/std_as_linestring.hpp>

#include <vector>
#include <deque>


template <typename G, typename Expected>
void test_geometry()
{
    BOOST_CHECK_EQUAL(typeid(typename ggl::point_type<G>::type).name(),
        typeid(Expected).name());
}

template <typename P>
void test_all()
{
    test_geometry<P, P>();
    test_geometry<const P, P>();
    test_geometry<ggl::linestring<P> , P>();
    test_geometry<ggl::linear_ring<P> , P>();
    test_geometry<ggl::polygon<P> , P>();
    test_geometry<ggl::box<P> , P>();
    test_geometry<ggl::segment<P> , P>();
    test_geometry<ggl::segment<const P> , P>();
    test_geometry<ggl::nsphere<P, double> , P>();

    test_geometry<std::vector<P>, P>();
    test_geometry<std::deque<P>, P>();

    test_geometry<boost::array<P, 5>, P>();
}

int test_main(int, char* [])
{
    test_geometry<int[2], int[2]>();
    test_geometry<float[2], float[2]>();
    test_geometry<double[2], double[2]>();

    test_geometry<int[3], int[3]>();
    test_geometry<float[3], float[3]>();
    test_geometry<double[3], double[3]>();

    test_geometry<boost::tuple<double, double>,
                boost::tuple<double, double> >();
    test_geometry<boost::tuple<double, double, double>,
                boost::tuple<double, double, double> >();

    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
