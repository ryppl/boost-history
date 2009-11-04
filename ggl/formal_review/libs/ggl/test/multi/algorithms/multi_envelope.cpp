// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <algorithms/test_envelope.hpp>


#include <ggl/multi/algorithms/envelope.hpp>
#include <ggl/multi/geometries/multi_point.hpp>
#include <ggl/multi/geometries/multi_linestring.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt_multi.hpp>

#include <ggl/geometries/geometries.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>
#include <test_common/test_point.hpp>




template <typename P>
void test_2d()
{
    /*test_envelope<ggl::multi_point<P> >(
            "MULTIPOINT((1 1),(1 0),(1 2))", 1, 1, 0, 2);
    test_envelope<ggl::multi_linestring<ggl::linestring<P> > >(
            "MULTILINESTRING((0 0,1 1),(1 1,2 2),(2 2,3 3))", 0, 3, 0, 3);
*/
    test_envelope<ggl::multi_polygon<ggl::polygon<P> > >(
            "MULTIPOLYGON(((1 1,1 3,3 3,3 1,1 1)),((4 4,4 6,6 6,6 4,4 4)))", 1, 6, 1, 6);
}


template <typename P>
void test_3d()
{
    typedef ggl::multi_point<P> mp;
}


int test_main( int , char* [] )
{
    test_2d<boost::tuple<float, float> >();
    test_2d<ggl::point_xy<float> >();
    test_2d<ggl::point_xy<double> >();

    test_3d<boost::tuple<float, float, float> >();
    test_3d<ggl::point<double, 3, ggl::cs::cartesian> >();

    return 0;
}
