// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_CONVEX_HULL_HPP
#define GGL_TEST_CONVEX_HULL_HPP

// Test-functionality, shared between single and multi tests


template <typename Geometry, typename Hull>
void test_convex_hull(Geometry const& geometry, Hull const& hull,
                      std::size_t size_original, std::size_t size_hull,
                      double expected_area, bool reverse)
{

    std::size_t n = ggl::num_points(hull);

    BOOST_CHECK_MESSAGE(n == size_hull,
        "convex hull: " << ggl::wkt(geometry) 
        << " -> " << ggl::wkt(hull) 
        << " type " 
        << (typeid(typename ggl::coordinate_type<Hull>::type).name())
        << " -> Expected: " << size_hull
        << " detected: " << n);
    

    BOOST_CHECK(ggl::num_points(geometry) == size_original);

    double ah = ggl::area(hull);
    if (reverse)
    {
        ah = -ah;
    }

//std::cout << "Area: " << ggl::area(geometry) << std::endl;
//std::cout << ggl::wkt(hull) << std::endl;

    BOOST_CHECK_CLOSE(ah, expected_area, 0.001);
}

template <typename Geometry, bool Clockwise>
void test_geometry_order(std::string const& wkt,
                      std::size_t size_original, std::size_t size_hull,
                      double expected_area)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);

    ggl::polygon
        <
            typename ggl::point_type<Geometry>::type,
            std::vector,
            std::vector,
            Clockwise
        > hull;

    // Test version with output iterator
    convex_hull_inserter(geometry, std::back_inserter(hull.outer()));
    test_convex_hull(geometry, hull,
        size_original, size_hull, expected_area, ! Clockwise);

    // Test version with ring as output
    ggl::clear(hull);
    convex_hull(geometry, hull.outer());
    test_convex_hull(geometry, hull, size_original, size_hull, expected_area, false);

    // Test version with polygon as output
    ggl::clear(hull);
    convex_hull(geometry, hull);
    test_convex_hull(geometry, hull, size_original, size_hull, expected_area, false);
}

template <typename Geometry>
void test_geometry(std::string const& wkt,
                      std::size_t size_original, std::size_t size_hull,
                      double expected_area)
{
    test_geometry_order<Geometry, true>(wkt, size_original, size_hull, expected_area);
    test_geometry_order<Geometry, false>(wkt, size_original, size_hull, expected_area);
}


#endif
