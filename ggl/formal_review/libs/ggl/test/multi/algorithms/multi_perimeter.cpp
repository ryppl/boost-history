// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/perimeter.hpp>
#include <ggl/multi/algorithms/perimeter.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/geometries/polygon.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>

#include <algorithms/test_perimeter.hpp>


template <typename P>
void test_all()
{
    test_geometry<ggl::multi_polygon<ggl::polygon<P> > >(
            "MULTIPOLYGON(((0 0,0 1,1 0,0 0)))", 1.0 + 1.0 + sqrt(2.0));
}

int test_main( int , char* [] )
{
    test_all<ggl::point_xy<double> >();

    return 0;
}
