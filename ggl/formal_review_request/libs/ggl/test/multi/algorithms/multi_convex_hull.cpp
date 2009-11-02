// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <iterator>
#include <string>

#include <algorithms/test_convex_hull.hpp>

#include <ggl/geometries/geometries.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>

#include <ggl/multi/core/point_type.hpp>

#include <ggl/multi/iterators/range_type.hpp>

#include <ggl/multi/algorithms/num_points.hpp>
#include <ggl/multi/algorithms/convex_hull.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/multi/geometries/multi_point.hpp>
#include <ggl/multi/geometries/multi_linestring.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>




template <typename P>
void test_all()
{
    typedef ggl::multi_point<P> mp;
    typedef ggl::multi_linestring<ggl::linestring<P> > ml;
    typedef ggl::multi_polygon<ggl::polygon<P> > mpoly;
    test_geometry<mp>("multipoint((1.1 1.1), (2.5 2.1), (3.1 3.1), (4.9 1.1), (3.1 1.9))", 5, 4, 3.8);
    test_geometry<ml>("multilinestring((2 4, 3 4, 3 5), (4 3,4 4,5 4))", 6, 5, 3.0);
    test_geometry<mpoly>("multipolygon(((1 4,1 6,2 5,3 5,4 6,4 4,1 4)), ((4 2,4 3,6 3,6 2,4 2)))", 12, 7, 14.0);
}


int test_main(int, char* [])
{
    //test_all<ggl::point_xy<int> >();
    //test_all<ggl::point_xy<float> >();
    test_all<ggl::point_xy<double> >();

    return 0;
}
