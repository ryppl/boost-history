// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Custom Triangle Example

#include <iostream>

#include <boost/array.hpp>

#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/centroid.hpp>
#include <ggl/geometries/register/ring.hpp>
#include <ggl/strategies/strategies.hpp>
#include <ggl/util/write_dsv.hpp>


struct triangle : public boost::array<ggl::point_xy<double>, 4>
{
    inline void close()
    {
        (*this)[3] = (*this)[0];
    }
};


GEOMETRY_REGISTER_RING(triangle)


// Specializations of algorithms, where useful. If not specialized the default ones
// (for linear rings) will be used for triangle. Which is OK as long as the triangle
// is closed, that means, has 4 points (the last one being the first).
namespace ggl {

template<>
inline double area<triangle>(const triangle& t)
{
    /*         C
              / \
             /   \
            A-----B

           ((Bx - Ax) * (Cy - Ay)) - ((Cx - Ax) * (By - Ay))
           -------------------------------------------------
                                   2
    */

    return 0.5 * ((t[1].x() - t[0].x()) * (t[2].y() - t[0].y())
                - (t[2].x() - t[0].x()) * (t[1].y() - t[0].y()));
}

} // namespace ggl

int main()
{
    triangle t;

    t[0].x(0);
    t[0].y(0);
    t[1].x(5);
    t[1].y(0);
    t[2].x(2.5);
    t[2].y(2.5);

    t.close();

    std::cout << "Triangle: " << ggl::dsv(t) << std::endl;
    std::cout << "Area: " << ggl::area(t) << std::endl;

    ggl::point_xy<double> c;
    ggl::centroid(t, c);
    std::cout << "Centroid: " << ggl::dsv(c) << std::endl;

    return 0;
}
