// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/area.hpp>
#include <ggl/multi/algorithms/area.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/geometries/polygon.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>

#include <algorithms/test_area.hpp>


template <typename P>
void test_all()
{
    typedef ggl::multi_polygon<ggl::polygon<P> > mp;
    test_geometry<mp>("MULTIPOLYGON(((0 0,0 7,4 2,2 0,0 0)))", 16.0);
}

int test_main( int , char* [] )
{
    test_all<ggl::point_xy<double> >();

    return 0;
}
