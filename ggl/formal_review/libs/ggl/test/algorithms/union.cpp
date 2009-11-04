// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

#include <algorithms/test_union.hpp>
#include <algorithms/test_overlay.hpp>


template <typename P>
void test_all()
{
    typedef ggl::polygon<P> polygon;
    typedef ggl::linear_ring<P> ring;
    typedef ggl::box<P> box;

    test_one<polygon, box, polygon>(1, example_box, example_ring,
        1, 1, 15, 6.38875);

    test_one<polygon, box, polygon>(2, example_box, example_polygon,
        1, 3, 23, 6.30983);

    test_one<polygon, polygon, polygon>(3, example_star, example_ring,
        1, 0, 23, 5.67017141);

    test_one<polygon, polygon, polygon>(4, example_star, example_polygon,
        1, 1, 27,  5.647949);

    test_one<polygon, box, polygon>(5, example_box,
            "POLYGON((3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2))",
                1, 1, 15, 6.38875);
    test_one<polygon, box, polygon>(6, example_box,
            "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,5.3 2.5,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3))",
                1, 1, 13, 5.93625);

    // Same as 3, but now as polygon
    // (note, internally, the intersection points is different, so yes,
    // it has to be tested)
    test_one<polygon, polygon, polygon>(7, "POLYGON((1.5 1.5 , 1.5 2.5 , 4.5 2.5 , 4.5 1.5 , 1.5 1.5))",
            "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,5.3 2.5,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3))",
                1, 1, 13, 5.93625);

    test_one<polygon, box, polygon>(8, example_box,
            "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,4.5 2.5,4.5 1.2,4.9 0.8,2.9 0.7,2 1.3))",
                1, 1, 13, 4.651245);

    test_one<polygon, box, polygon>(9, example_box,
            "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,4.5 2.5,4.5 2.3,5.0 2.3,5.0 2.1,4.5 2.1,4.5 1.9,4.0 1.9,4.5 1.2,4.9 0.8,2.9 0.7,2 1.3))",
                1, 1, 19, 4.7191);

    test_one<polygon, box, polygon>(10, example_box,
            "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,4.5 1.2,2.9 0.7,2 1.3))",
                1, 1, 14, 4.2174);

    test_one<polygon, box, polygon>(11, example_box,
            "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.0 3.0,5.0 2.0,2.9 0.7,2 1.3))",
                1, 1, 16, 4.270554);

    test_one<polygon, box, polygon>(12, "box(0 0, 3 3)",
            "POLYGON((2 2, 1 4, 2 4, 3 3, 2 2))", 
                1, 0, 8, 10.25);


    // First inside second
    test_one<polygon, polygon, polygon>(13, 
        first_within_second[0], first_within_second[1],
        1, 0, 5, 25.0);

    // Second inside first
    test_one<polygon, polygon, polygon>(14, 
        first_within_second[1], first_within_second[0],
        1, 0, 5, 25.0);

    // First inside hole of second
    test_one<polygon, polygon, polygon>(15, 
        first_within_hole_of_second[0], first_within_hole_of_second[1],
        2, 1, 15, 17.0);

    // forming new hole
    test_one<polygon, polygon, polygon>(16, 
        new_hole[0], new_hole[1],
        1, 1, 14, 23.0);

    // side by side
    test_one<polygon, polygon, polygon>(17, 
        side_side[0], side_side[1], 1, 0, 6, 2.0);

    // identical
    test_one<polygon, polygon, polygon>(18, 
        identical[0], identical[1], 1, 0, 5, 1.0);

    // inside each other, having intersections; holes separate intersections
    test_one<polygon, polygon, polygon>(19, 
        intersect_holes_intersect[0], intersect_holes_intersect[1],
        1, 1, 14, 39.75);

    test_one<polygon, polygon, polygon>(20, 
        intersect_holes_intersect_and_disjoint[0], intersect_holes_intersect_and_disjoint[1],
        1, 1, 14, 39.75);

    test_one<polygon, polygon, polygon>(21, 
        intersect_holes_intersect_and_touch[0], intersect_holes_intersect_and_touch[1],
        1, 1, 14, 39.75);

    // inside each other, having intersections but holes are disjoint
    test_one<polygon, polygon, polygon>(22, 
        intersect_holes_disjoint[0],
        intersect_holes_disjoint[1], 
        1, 0, 9, 40.0);

    // inside each other, having no intersections but holes are disjoint
    test_one<polygon, polygon, polygon>(23, 
        within_holes_disjoint[0], within_holes_disjoint[1], 
        1, 0, 5, 49.0);

    test_one<polygon, polygon, polygon>(24, 
        winded[0], winded[1], 
        1, 5, 30, 114.0);

    test_one<polygon, polygon, polygon>(25, 
        two_bends[0], two_bends[1],
        1, 0, 7, 40.0);


    // test some other input/output types

    // 1 input ring
    test_one<polygon, polygon, ring>(31, identical[0], identical[1], 1, 0, 5, 1.0);
    test_one<polygon, ring, polygon>(32, identical[0], identical[1], 1, 0, 5, 1.0);

    // 2 input rings
    test_one<polygon, ring, ring>(33, identical[0], identical[1], 1, 0, 5, 1.0);

    // output is also ring
    test_one<ring, ring, ring>(34, identical[0], identical[1], 1, 0, 5, 1.0);

    // "new hole", tested with ring -> the newly formed hole will be omitted
    test_one<ring, ring, ring>(35, new_hole[0], new_hole[1], 1, 0, 9, 24.0);

}


int test_main(int, char* [])
{
    test_all<ggl::point_xy<double> >();
    return 0;
}
