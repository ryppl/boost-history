// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <algorithms/test_envelope.hpp>

#include <ggl/geometries/geometries.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>
#include <ggl/geometries/adapted/std_pair_as_segment.hpp>
#include <test_common/test_point.hpp>



template <typename P>
void test_2d()
{
    test_envelope<P>("POINT(1 1)", 1, 1, 1, 1);
    test_envelope<ggl::linestring<P> >("LINESTRING(1 1,2 2)", 1, 2, 1, 2);
    test_envelope<ggl::polygon<P> >("POLYGON((1 1,1 3,3 3,3 1,1 1))", 1, 3, 1, 3);

    test_envelope<ggl::linear_ring<P> >("POLYGON((1 1,1 3,3 3,3 1,1 1))", 1, 3, 1, 3);
    test_envelope<ggl::box<P> >("BOX(1 1,3 3)", 1, 3, 1, 3);

    typedef std::pair<P, P> segment_type;
    test_envelope<segment_type>("SEGMENT(1 1,3 3)", 1, 3, 1, 3);
}

template <typename P>
void test_3d()
{
    test_envelope<P>("POINT(1 2 3)", 1, 1, 2, 2, 3, 3);
    test_envelope<P>("POINT(3 2 1)", 3, 3, 2, 2, 1, 1);
    test_envelope<ggl::linestring<P> >("LINESTRING(1 1 1,2 2 2)", 1, 2, 1, 2, 1, 2);
    test_envelope<ggl::box<P> >("BOX(1 1 1,3 3 3)", 1, 3, 1, 3, 1, 3);
}


int test_main(int, char* [])
{
    //test_2d<int[2]>();
    //test_2d<float[2]>();
    //test_2d<double[2]>();
    test_2d<boost::tuple<float, float> >();
    test_2d<ggl::point_xy<int> >();
    test_2d<ggl::point_xy<float> >();
    test_2d<ggl::point_xy<double> >();

    test_3d<test::test_point>();

    return 0;
}
