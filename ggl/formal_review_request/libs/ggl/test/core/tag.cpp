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
    BOOST_CHECK_EQUAL(typeid(typename ggl::tag<G>::type).name(),
        typeid(Expected).name());
}

template <typename P, size_t D>
void test_all()
{
    test_geometry<P, ggl::point_tag>();
    test_geometry<const P, ggl::point_tag>();
    test_geometry<ggl::linestring<P> , ggl::linestring_tag>();
    test_geometry<ggl::linear_ring<P> , ggl::ring_tag>();
    test_geometry<ggl::polygon<P> , ggl::polygon_tag>();
    test_geometry<ggl::box<P> , ggl::box_tag>();
    test_geometry<ggl::segment<P> , ggl::segment_tag>();
    test_geometry<ggl::segment<const P> , ggl::segment_tag>();
    test_geometry<ggl::nsphere<P, double> , ggl::nsphere_tag>();

    test_geometry<std::vector<P>, ggl::linestring_tag>();
    test_geometry<std::deque<P>, ggl::linestring_tag>();

    test_geometry<boost::array<P, 5>, ggl::linestring_tag>();
}

int test_main(int, char* [])
{
    test_geometry<int[2], ggl::point_tag>();
    test_geometry<float[2], ggl::point_tag>();
    test_geometry<double[2], ggl::point_tag>();

    test_geometry<int[3], ggl::point_tag>();
    test_geometry<float[3], ggl::point_tag>();
    test_geometry<double[3], ggl::point_tag>();

    test_geometry<boost::tuple<double, double>, ggl::point_tag>();
    test_geometry<boost::tuple<double, double, double>, ggl::point_tag>();

    test_all<ggl::point<int, 2, ggl::cs::cartesian>, 2 >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian>, 2 >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian>, 2 >();

    return 0;
}
