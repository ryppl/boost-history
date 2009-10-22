// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/simplify.hpp>
#include <ggl/multi/algorithms/simplify.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/geometries/box.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/linestring.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/polygon.hpp>

#include <ggl/multi/geometries/multi_point.hpp>
#include <ggl/multi/geometries/multi_linestring.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>

#include <algorithms/test_simplify.hpp>


template <typename P>
void test_all()
{
    test_geometry<ggl::multi_point<P> >(
        "MULTIPOINT((0 0),(1 1))",
        "MULTIPOINT((0 0),(1 1))", 1.0);

    test_geometry<ggl::multi_linestring<ggl::linestring<P> > >(
        "MULTILINESTRING((0 0,5 5,10 10))",
        "MULTILINESTRING((0 0,10 10))", 1.0);

    typedef ggl::multi_polygon<ggl::polygon<P> > mp;
    test_geometry<mp>(
        "MULTIPOLYGON(((4 0,8 2,8 7,4 9,0 7,0 2,2 1,4 0)))",
        "MULTIPOLYGON(((4 0,8 2,8 7,4 9,0 7,0 2,4 0)))", 1.0);
    test_geometry<mp>(
        "MULTIPOLYGON(((4 0,8 2,8 7,4 9,0 7,0 2,2 1,4 0),(7 3,7 6,1 6,1 3,4 3,7 3)))",
        "MULTIPOLYGON(((4 0,8 2,8 7,4 9,0 7,0 2,4 0),(7 3,7 6,1 6,1 3,7 3)))", 1.0);
}

int test_main( int , char* [] )
{
    test_all<ggl::point_xy<double> >();

    return 0;
}
