// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/for_each.hpp>
#include <ggl/multi/algorithms/for_each.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/geometries/box.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/linestring.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/polygon.hpp>

#include <ggl/multi/geometries/multi_point.hpp>
#include <ggl/multi/geometries/multi_linestring.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>

#include <algorithms/test_for_each.hpp>


template <typename P>
void test_all()
{
    test_geometry<ggl::multi_point<P> >
        (
            "MULTIPOINT((1 1))"

            // per point
            , 1
            , "MULTIPOINT((101 1))"
            , "MULTIPOINT((101 100))"
            // per segment
            , ""
            , 0
            , "MULTIPOINT((1 1))"
        );

    test_geometry<ggl::multi_linestring<ggl::linestring<P> > >
        (
        "MULTILINESTRING((1 1,2 2))"

            , 3
            , "MULTILINESTRING((101 1,102 2))"
            , "MULTILINESTRING((101 100,102 200))"

            , "((1, 1), (2, 2))"
            , std::sqrt(2.0)
            , "MULTILINESTRING((10 1,2 2))"
        );

    typedef ggl::multi_polygon<ggl::polygon<P> > mp;
    test_geometry<mp>
        (
            "MULTIPOLYGON(((1 1,1 4,4 4,4 1,1 1)))"

            , 11
            , "MULTIPOLYGON(((101 1,101 4,104 4,104 1,101 1)))"
            , "MULTIPOLYGON(((101 100,101 400,104 400,104 100,101 100)))"

            , "((1, 1), (1, 4)) ((1, 4), (4, 4)) ((4, 4), (4, 1)) ((4, 1), (1, 1))"
            , 4 * 3.0
            , "MULTIPOLYGON(((10 1,10 4,4 4,4 1,1 1)))"
        );
}

int test_main( int , char* [] )
{
    test_all<ggl::point_xy<double> >();

    return 0;
}
