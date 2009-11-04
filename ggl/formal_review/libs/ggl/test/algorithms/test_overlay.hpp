// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_OVERLAY_HPP
#define GGL_TEST_OVERLAY_HPP

#include <string>

static std::string example_box = "box(1.5 1.5, 4.5 2.5)";
static std::string example_ring =
    "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3))";

static std::string example_polygon =
    "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))";



static std::string example_star =
    "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))";

static std::string identical[2] =
    {"POLYGON((0 0,0 1,1 1,1 0,0 0))",
    "POLYGON((1 1,1 0,0 0,0 1,1 1))"};


static std::string new_hole[2] =
    {"POLYGON((2 2,2 5,5 5,5 2,2 2))",
    "POLYGON((0 0,0 6,3 6,3 4,1 4,1 3,3 3,3 0,0 0))"};

static std::string two_bends[2] =
    {"POLYGON((0 4,4 8,7 7,8 4,5 3,4 0,0 4))", 
    "POLYGON((0 4,4 8,5 5,8 4,7 1,4 0,0 4))"};

static std::string side_side[2] =
    {"POLYGON((0 0,0 1,1 1,1 0,0 0))",
    "POLYGON((1 0,1 1,2 1,2 0,1 0))"};


    // First within second
static std::string first_within_second[2] =
    {"POLYGON((2 2,2 3,3 3,3 2,2 2))",
    "POLYGON((0 0, 0 5, 5 5, 5 0, 0 0))"};


    // First within hole of second
static std::string first_within_hole_of_second[2] =
    {"POLYGON((2 2,2 3,3 3,3 2,2 2))",
    "POLYGON((0 0, 0 5, 5 5, 5 0, 0 0),(1 1,4 1,4 4,1 4,1 1))"};


// within each other, having no intersections but many holes within each other
static std::string winded[2] =
    {"POLYGON((0 0,0 11,11 11,11 0,0 0),(3 3,4 3,4 4,3 4,3 3),(5 3,6 3,6 4,5 4,5 3),(2 6,7 6,7 6,7 9,2 9,2 6),(9 2,10 2,10 5,9 5,9 2))",
    "POLYGON((1 1,1 10,10 10,10 6,8 6,8 1,1 1),(2 2,7 2,7 5,2 5,2 2),(3 7,4 7,4 8,3 8,3 7),(5 7,6 7,6 8,5 8,5 7),(8 7,9 7,9 8,8 8,8 7))"};

static std::string intersect_holes_disjoint[2] =
    {"POLYGON((0 0,0 7,5 7,5 0,0 0),(2 2,3 2,3 3,2 3,2 2))",
    "POLYGON((1 1,1 6,6 6,6 1,1 1),(2 4,3 4,3 5,2 5,2 4))"};

static std::string within_holes_disjoint[2] =
    {"POLYGON((0 0,0 7,7 7,7 0,0 0),(2 2,3 2,3 3,2 3,2 2))",
    "POLYGON((1 1,1 6,6 6,6 1,1 1),(2 4,3 4,3 5,2 5,2 4))"};

static std::string intersect_holes_intersect[2] =
    {"POLYGON((0 0,0 7,5 7,5 0,0 0),(2 2,3 2,3 3,2 3,2 2))",
            "POLYGON((1 1,1 6,6 6,6 1,1 1),(2.5 2.5,3.5 2.5,3.5 3.5,2.5 3.5,2.5 2.5))"};

static std::string intersect_holes_intersect_and_disjoint[2] =
    {"POLYGON((0 0,0 7,5 7,5 0,0 0),(2 2,3 2,3 3,2 3,2 2),(2 4,3 4,3 5,2 5,2 4))",
            "POLYGON((1 1,1 6,6 6,6 1,1 1),(2.5 2.5,3.5 2.5,3.5 3.5,2.5 3.5,2.5 2.5))"};


static std::string intersect_holes_intersect_and_touch[2] =
    {"POLYGON((0 0,0 7,5 7,5 0,0 0),(2 2,3 2,3 3,2 3,2 2),(2.5 4,3 4.5,2.5 5,2 4.5,2.5 4))",
            "POLYGON((1 1,1 6,6 6,6 1,1 1),(2.5 2.5,3.5 2.5,3.5 3.5,2.5 3.5,2.5 2.5),(3.5 4,4 4.5,3.5 5,3 4.5,3.5 4))"};


#endif
