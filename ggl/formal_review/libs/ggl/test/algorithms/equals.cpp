// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithms/test_equals.hpp>

#include <ggl/geometries/geometries.hpp>


template <typename P>
void test_all()
{
    typedef ggl::box<P> box;
    typedef ggl::linear_ring<P> ring;
    typedef ggl::polygon<P> polygon;


    test_geometry<P, P>("POINT(1 1)", "POINT(1 1)", true);
    test_geometry<P, P>("POINT(1 1)", "POINT(1 2)", false);
    test_geometry<box, box>("BOX(1 1,2 2)", "BOX(1 2,2 2)", false);
    test_geometry<box, box>("BOX(1 2,3 4)", "BOX(1 2,3 4)", true);

    // Completely equal
    test_geometry<ring, ring>("POLYGON((0 0,0 2,2 2,0 0))", "POLYGON((0 0,0 2,2 2,0 0))", true);
    // Shifted
    test_geometry<ring, ring>("POLYGON((2 2,0 0,0 2,2 2))", "POLYGON((0 0,0 2,2 2,0 0))", true);
    // Extra coordinate
    test_geometry<ring, ring>("POLYGON((0 0,0 2,2 2,0 0))", "POLYGON((0 0,0 2,2 2,1 1,0 0))", true);
    // Degenerate points
    test_geometry<ring, ring>("POLYGON((0 0,0 2,2 2,2 2,0 0))", "POLYGON((0 0,0 2,0 2,2 2,0 0))", true);

    // Two different bends, same area, unequal
    test_geometry<ring, ring>(
        "POLYGON((4 0,5 3,8 4,7 7,4 8,0 4,4 0))", 
        "POLYGON((4 0,7 1,8 4,5 5,4 8,0 4,4 0))", false);

    // Unequal (but same area)    
    test_geometry<ring, ring>("POLYGON((0 0,0 2,2 2,0 0))", "POLYGON((1 1,1 3,3 3,1 1))", false);

    // Polygons, exterior rings equal (shifted)
    test_geometry<polygon, polygon>("POLYGON((0 0,0 2,2 2,0 0))", "POLYGON((0 2,2 2,0 0,0 2))", true);

    // One having hole
    test_geometry<polygon, polygon>("POLYGON((0 0,0 4,4 4,0 0))", "POLYGON((0 0,0 4,4 4,0 0),(1 1,2 1,2 2,1 2,1 1))", false);

    // Both having holes
    test_geometry<polygon, polygon>(
            "POLYGON((0 0,0 4,4 4,0 0),(1 1,2 1,2 2,1 2,1 1))", 
            "POLYGON((0 0,0 4,4 4,0 0),(1 1,2 1,2 2,1 2,1 1))", true);

    // Both having holes, outer equal, inner not equal
    test_geometry<polygon, polygon>(
            "POLYGON((0 0,0 4,4 4,0 0),(1 1,2 1,2 2,1 2,1 1))", 
            "POLYGON((0 0,0 4,4 4,0 0),(2 2,3 2,3 3,2 3,2 2))", false);

    // Both having 2 holes, equal but in different order
    test_geometry<polygon, polygon>(
            "POLYGON((0 0,0 4,4 4,0 0),(1 1,2 1,2 2,1 2,1 1),(2 2,3 2,3 3,2 3,2 2))", 
            "POLYGON((0 0,0 4,4 4,0 0),(2 2,3 2,3 3,2 3,2 2),(1 1,2 1,2 2,1 2,1 1))", true);

    // Both having 5 holes, equal but in different order
    test_geometry<polygon, polygon>(
            "POLYGON((0 0,0 10,10 10,0 0),(1 1,2 1,2 2,1 2,1 1),(4 1,5 1,5 2,4 2,4 1),(2 2,3 2,3 3,2 3,2 2))", 
            "POLYGON((0 0,0 10,10 10,0 0),(4 1,5 1,5 2,4 2,4 1),(2 2,3 2,3 3,2 3,2 2),(1 1,2 1,2 2,1 2,1 1))", true);
}




int test_main( int , char* [] )
{
    //test_all<ggl::point_xy<int> >();
    test_all<ggl::point_xy<double> >();

#if defined(HAVE_CLN)
//    test_all<ggl::point_xy<boost::numeric_adaptor::cln_value_type> >();
#endif
#if defined(HAVE_GMP)
//    test_all<ggl::point_xy<boost::numeric_adaptor::gmp_value_type> >();
#endif

    return 0;
}
