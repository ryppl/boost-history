// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>


#include <ggl/core/coordinate_system.hpp>


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
    BOOST_CHECK_EQUAL(typeid(typename ggl::coordinate_system<G>::type).name(),
        typeid(Expected).name());
}

template <typename P, typename Expected>
void test_all()
{
    test_geometry<P, Expected>();
    test_geometry<const P, Expected>();
    test_geometry<ggl::linestring<P> , Expected>();
    test_geometry<ggl::linear_ring<P> , Expected>();
    test_geometry<ggl::polygon<P> , Expected>();
    test_geometry<ggl::box<P> , Expected>();
    test_geometry<ggl::segment<P> , Expected>();
    test_geometry<ggl::segment<const P> , Expected>();

    test_geometry<std::vector<P>, Expected>();
    test_geometry<std::deque<P>, Expected>();

    test_geometry<boost::array<P, 5>, Expected>();
}

int test_main(int, char* [])
{
    // Because of the included headerfiles, there are always cartesian
    test_geometry<int[2], ggl::cs::cartesian>();
    test_geometry<float[2], ggl::cs::cartesian>();
    test_geometry<double[2], ggl::cs::cartesian>();

    test_geometry<int[3], ggl::cs::cartesian>();
    test_geometry<float[3], ggl::cs::cartesian>();
    test_geometry<double[3], ggl::cs::cartesian>();

    // Because of the included headerfiles, there are always cartesian
    test_geometry<boost::tuple<double, double>, ggl::cs::cartesian>();
    test_geometry<boost::tuple<double, double, double>, ggl::cs::cartesian>();


    // Test cartesian
    test_all<ggl::point<int, 2, ggl::cs::cartesian>, ggl::cs::cartesian>();
    test_all<ggl::point<float, 2, ggl::cs::cartesian>, ggl::cs::cartesian>();
    test_all<ggl::point<double, 2, ggl::cs::cartesian>, ggl::cs::cartesian>();

    // Test spherical
    test_all<ggl::point<int, 2, ggl::cs::spherical<ggl::degree> >,
            ggl::cs::spherical<ggl::degree> >();
    test_all<ggl::point<float, 2, ggl::cs::spherical<ggl::degree> >,
            ggl::cs::spherical<ggl::degree> >();
    test_all<ggl::point<double, 2, ggl::cs::spherical<ggl::degree> >,
            ggl::cs::spherical<ggl::degree> >();

    test_all<ggl::point<int, 2, ggl::cs::spherical<ggl::radian> >,
                ggl::cs::spherical<ggl::radian> >();
    test_all<ggl::point<float, 2, ggl::cs::spherical<ggl::radian> >,
                ggl::cs::spherical<ggl::radian> >();
    test_all<ggl::point<double, 2, ggl::cs::spherical<ggl::radian> >,
                ggl::cs::spherical<ggl::radian> >();

    // Test other
    test_all<ggl::point<double, 2, ggl::cs::polar<ggl::degree> >,
            ggl::cs::polar<ggl::degree> >();

    test_all<ggl::point<double, 2, ggl::cs::geographic<ggl::degree> >,
            ggl::cs::geographic<ggl::degree> >();

    return 0;
}
