// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_AREA_HPP
#define GGL_TEST_AREA_HPP

// Test-functionality, shared between single and multi tests


template <typename Geometry>
void test_area(Geometry const& geometry, long double expected_area)
{
    long double area = ggl::area(geometry);

#ifdef GGL_TEST_DEBUG
    std::ostringstream out;
    out << typeid(typename ggl::coordinate_type<Geometry>::type).name()
        << std::endl
        << typeid(typename ggl::area_result<Geometry>::type).name()
        << std::endl
        << "area : " << ggl::area(geometry)
        << std::endl;
    std::cout << out.str();
#endif

    BOOST_CHECK_CLOSE(area, expected_area, 0.0001);

    // Test with explicitly defined strategies
    ggl::strategy::area::by_triangles
        <
            typename ggl::point_type<Geometry>::type
        > strategy1;

    area = ggl::area(geometry, strategy1);

    ggl::strategy::area::by_triangles
        <
            typename ggl::point_type<Geometry>::type,
            long double
        > strategy2;

    area = ggl::area(geometry, strategy2);
}


template <typename Geometry>
void test_geometry(std::string const& wkt, double expected_area)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);
    test_area(geometry, expected_area);
}


#endif
