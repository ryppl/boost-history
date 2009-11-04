// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <algorithms/test_area.hpp>

#include <ggl/multi/multi.hpp>

#include <ggl/geometries/geometries.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>




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
