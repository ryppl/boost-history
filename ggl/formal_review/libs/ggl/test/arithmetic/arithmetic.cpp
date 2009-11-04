// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>


#include <ggl/arithmetic/arithmetic.hpp>

#include <ggl/algorithms/assign.hpp>

#include <ggl/geometries/point.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>
#include <test_common/test_point.hpp>


using namespace ggl;

template <typename P>
void test_addition()
{
    P p1;
    ggl::assign(p1, 1, 2, 3);
    ggl::add_value(p1, 10);
    BOOST_CHECK(get<0>(p1) == 11);
    BOOST_CHECK(get<1>(p1) == 12);
    BOOST_CHECK(get<2>(p1) == 13);

    P p2;
    ggl::assign(p2, 4, 5, 6);
    ggl::add_point(p1, p2);
    BOOST_CHECK(get<0>(p1) == 15);
    BOOST_CHECK(get<1>(p1) == 17);
    BOOST_CHECK(get<2>(p1) == 19);
}

template <typename P>
void test_subtraction()
{
    P p1;
    ggl::assign(p1, 1, 2, 3);
    ggl::subtract_value(p1, 10);
    BOOST_CHECK(get<0>(p1) == -9);
    BOOST_CHECK(get<1>(p1) == -8);
    BOOST_CHECK(get<2>(p1) == -7);

    P p2;
    ggl::assign(p2, 4, 6, 8);
    ggl::subtract_point(p1, p2);
    BOOST_CHECK(get<0>(p1) == -13);
    BOOST_CHECK(get<1>(p1) == -14);
    BOOST_CHECK(get<2>(p1) == -15);
}

template <typename P>
void test_multiplication()
{
    P p1;
    ggl::assign(p1, 1, 2, 3);
    ggl::multiply_value(p1, 5);
    BOOST_CHECK(get<0>(p1) == 5);
    BOOST_CHECK(get<1>(p1) == 10);
    BOOST_CHECK(get<2>(p1) == 15);

    P p2;
    ggl::assign(p2, 4, 5, 6);
    ggl::multiply_point(p1, p2);
    BOOST_CHECK(get<0>(p1) == 20);
    BOOST_CHECK(get<1>(p1) == 50);
    BOOST_CHECK(get<2>(p1) == 90);
}

template <typename P>
void test_division()
{
    P p1;
    ggl::assign(p1, 50, 100, 150);
    ggl::divide_value(p1, 5);
    BOOST_CHECK(get<0>(p1) == 10);
    BOOST_CHECK(get<1>(p1) == 20);
    BOOST_CHECK(get<2>(p1) == 30);

    P p2;
    ggl::assign(p2, 2, 4, 6);
    ggl::divide_point(p1, p2);
    BOOST_CHECK(get<0>(p1) == 5);
    BOOST_CHECK(get<1>(p1) == 5);
    BOOST_CHECK(get<2>(p1) == 5);
}


template <typename P>
void test_all()
{
    test_addition<P>();
    test_subtraction<P>();
    test_multiplication<P>();
    test_division<P>();
}


int test_main(int, char* [])
{
    test_all<int[3]>();
    test_all<float[3]>();
    test_all<double[3]>();
    test_all<test::test_point>();
    test_all<point<int, 3, ggl::cs::cartesian> >();
    test_all<point<float, 3, ggl::cs::cartesian> >();
    test_all<point<double, 3, ggl::cs::cartesian> >();

    return 0;
}
