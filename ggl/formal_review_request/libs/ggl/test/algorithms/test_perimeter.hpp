// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_PERIMETER_HPP
#define GGL_TEST_PERIMETER_HPP

// Test-functionality, shared between single and multi tests


template <typename Geometry>
void test_perimeter(Geometry const& geometry, long double expected_perimeter)
{
    long double perimeter = ggl::perimeter(geometry);

#ifdef GGL_TEST_DEBUG
    std::ostringstream out;
    out << typeid(typename ggl::coordinate_type<Geometry>::type).name() 
        << std::endl
        << typeid(typename ggl::perimeter_result<Geometry>::type).name() 
        << std::endl
        << "perimeter : " << ggl::perimeter(geometry) 
        << std::endl;
    std::cout << out.str();
#endif

    BOOST_CHECK_CLOSE(perimeter, expected_perimeter, 0.0001);
}


template <typename Geometry>
void test_geometry(std::string const& wkt, double expected_perimeter)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);
    test_perimeter(geometry, expected_perimeter);
}


#endif
