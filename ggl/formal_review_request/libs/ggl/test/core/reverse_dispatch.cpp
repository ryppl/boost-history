// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/test/included/test_exec_monitor.hpp>

#include <ggl/core/reverse_dispatch.hpp>

#include <ggl/geometries/geometries.hpp>



template <typename Geometry1, typename Geometry2, bool Expected>
void test_reversed()
{
    BOOST_CHECK_EQUAL((ggl::reverse_dispatch<Geometry1, Geometry2>::type::value),
                Expected);
}


template <typename P>
void test_all()
{

    test_reversed<P, P, false>();
    test_reversed<P, ggl::linestring<P>, false>();
    test_reversed<ggl::linestring<P>, P, true>();
    test_reversed<ggl::linear_ring<P>, P, true>();
    test_reversed<ggl::linestring<P>, ggl::linear_ring<P>, false>();
    test_reversed<ggl::linear_ring<P>, ggl::linestring<P>, true>();
}

template <typename P1, typename P2>
void test_mixed()
{
    test_reversed<P1, P2, false>();
}


int test_main(int, char* [])
{
    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_mixed
        <
            ggl::point<int, 2, ggl::cs::cartesian>,
            ggl::point<int, 2, ggl::cs::spherical<ggl::degree> >
        >();
    test_mixed
        <
            ggl::point<int, 2, ggl::cs::spherical<ggl::degree> >,
            ggl::point<int, 2, ggl::cs::spherical<ggl::radian> >
        >();
    return 0;
}
