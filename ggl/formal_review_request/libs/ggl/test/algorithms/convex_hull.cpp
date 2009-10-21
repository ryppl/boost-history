// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include <ggl_test_common.hpp>

#include <ggl/algorithms/convex_hull.hpp>


#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/num_points.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>
#include <ggl/extensions/gis/io/wkt/write_wkt.hpp>

#include <ggl/geometries/geometries.hpp>

#include <algorithms/test_convex_hull.hpp>


template <typename P>
void test_all()
{
    // from sample linestring
    
    test_geometry<ggl::linestring<P> >(
        "linestring(1.1 1.1, 2.5 2.1, 3.1 3.1, 4.9 1.1, 3.1 1.9)", 5, 4, 3.8);

    // rectangular, with concavity
    test_geometry<ggl::polygon<P> >(
        "polygon((1 1, 5 1, 5 4, 4 4, 4 3, 3 3, 3 4, 1 4, 1 1))",
                9, 5, 12.0);

    // from sample polygon, with concavity
    test_geometry<ggl::polygon<P> >(
        "polygon((2.0 1.3, 2.4 1.7, 2.8 1.8, 3.4 1.2, 3.7 1.6,3.4 2.0, 4.1 3.0"
        ", 5.3 2.6, 5.4 1.2, 4.9 0.8, 2.9 0.7,2.0 1.3))",
                12, 8, 5.245);
}

int test_main(int, char* [])
{
    //test_all<ggl::point_xy<int> >();
    //test_all<ggl::point_xy<float> >();
    test_all<ggl::point_xy<double> >();

#if defined(HAVE_CLN)
    test_all<ggl::point_xy<boost::numeric_adaptor::cln_value_type> >();
#endif
#if defined(HAVE_GMP)
    // GMP compiles but gives errors in results
    // TODO: find out what's going wrong here
    // test_all<ggl::point_xy<boost::numeric_adaptor::gmp_value_type> >();
#endif

    return 0;
}
