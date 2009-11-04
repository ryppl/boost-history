// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Transformation Example

#include <iostream>

#include <ggl/ggl.hpp>
#include <ggl/geometries/cartesian2d.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/std_as_linestring.hpp>

int main()
{
    using namespace ggl;

    point_2d p(1, 1);
    point_2d p2;

    // Example: translate a point over (5,5)
    strategy::transform::translate_transformer<point_2d, point_2d> translate(5, 5);

    transform(p, p2, translate);
    std::cout << "transformed point " << ggl::dsv(p2) << std::endl;

    // Transform a polygon
    polygon_2d poly, poly2;
    const double coor[][2] = { {0, 0}, {0, 7}, {2, 2}, {2, 0}, {0, 0} };
    // note that for this syntax you have to include the two
    // include files above (c_array_cartesian.hpp, std_as_linestring.hpp)
    assign(poly, coor);
    //read_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
    transform(poly, poly2, translate);

    std::cout << "source      polygon " << ggl::dsv(poly) << std::endl;
    std::cout << "transformed polygon " << ggl::dsv(poly2) << std::endl;

    // Many more transformations are possible:
    // - from Cartesian to Spherical coordinate systems and back
    // - from Cartesian to Cartesian (mapping, affine transformations) and back (inverse)
    // - Map Projections
    // - from Degree to Radian and back in spherical or geographic coordinate systems

    return 0;
}
