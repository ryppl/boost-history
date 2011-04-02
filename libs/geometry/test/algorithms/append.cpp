// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2011 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2011 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2011 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <deque>
#include <vector>

#include <boost/concept/requires.hpp>
#include <geometry_test_common.hpp>

#include <boost/geometry/core/access.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include <boost/geometry/algorithms/clear.hpp>
#include <boost/geometry/algorithms/append.hpp>
#include <boost/geometry/algorithms/num_points.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/adapted/std_as_linestring.hpp>
#include <boost/geometry/geometries/adapted/boost_array_as_linestring.hpp>

#include <test_common/test_point.hpp>
#include <test_geometries/wrapped_boost_array.hpp>


template <typename G>
void test_geometry(bool check = true)
{
    G geometry;
    typedef typename bg::point_type<G>::type P;

    bg::append(geometry, bg::make_zero<P>());
    if (check)
    {
        BOOST_CHECK_EQUAL(bg::num_points(geometry), 1u);
    }

    // Append a range
    std::vector<P> v;
    v.push_back(bg::make_zero<P>());
    v.push_back(bg::make_zero<P>());
    bg::append(geometry, v);

    if (check)
    {
        BOOST_CHECK_EQUAL(bg::num_points(geometry), 3u);
    }

    bg::clear(geometry);

    if (check)
    {
        BOOST_CHECK_EQUAL(bg::num_points(geometry), 0u);
    }

    //P p = boost::range::front(geometry);
}

template <typename P>
void test_all()
{
    test_geometry<P>(false);
    test_geometry<bg::model::box<P> >(false);
    test_geometry<bg::model::segment<P> >(false);
    test_geometry<bg::model::linestring<P> >();
    test_geometry<bg::model::ring<P> >();
    test_geometry<bg::model::polygon<P> >();

    test_geometry<std::vector<P> >();
    test_geometry<std::deque<P> >();
    //test_geometry<std::list<P> >();

    test_geometry<test::wrapped_boost_array<P, 3> >();
}

int test_main(int, char* [])
{
    test_all<test::test_point>();
    test_all<bg::model::point<int, 2, bg::cs::cartesian> >();
    test_all<bg::model::point<float, 2, bg::cs::cartesian> >();
    test_all<bg::model::point<double, 2, bg::cs::cartesian> >();

#ifdef HAVE_TTMATH
    test_all<bg::model::point<ttmath_big, 2, bg::cs::cartesian> >();
#endif

    return 0;
}
