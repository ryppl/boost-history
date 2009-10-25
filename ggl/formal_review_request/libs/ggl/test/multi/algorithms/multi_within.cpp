// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/algorithms/correct.hpp>
#include <ggl/algorithms/within.hpp>

#include <ggl/geometries/box.hpp>
#include <ggl/geometries/polygon.hpp>

#include <ggl/multi/algorithms/within.hpp>

#include <ggl/multi/geometries/multi_polygon.hpp>

#include <algorithms/test_within.hpp>


template <typename P>
void test_all()
{
    typedef ggl::multi_polygon<ggl::polygon<P> > mp;

    // trivial cases
    test_geometry<P, mp>("POINT(1 1)", "MULTIPOLYGON(((0 0,0 2,2 2,2 0,0 0)))", true);
    test_geometry<P, mp>("POINT(3 3)", "MULTIPOLYGON(((0 0,0 2,2 2,2 0,0 0)))", false);

    // test if it is in one of them
    test_geometry<P, mp>("POINT(4 4)", "MULTIPOLYGON("
        "((0 0,0 2,2 2,2 0,0 0))"
        "((3 3,3 6,6 6,6 3,3 3))"
        ")", 
        true);
}

int test_main( int , char* [] )
{
    //test_all<ggl::point_xy<int> >();
    test_all<ggl::point_xy<double> >();

#if defined(HAVE_CLN)
    test_all<ggl::point_xy<boost::numeric_adaptor::cln_value_type> >();
#endif
#if defined(HAVE_GMP)
    test_all<ggl::point_xy<boost::numeric_adaptor::gmp_value_type> >();
#endif

    return 0;
}
