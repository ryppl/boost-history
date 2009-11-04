// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Custom point Example

#include <iostream>

#include <ggl/algorithms/distance.hpp>
#include <ggl/algorithms/make.hpp>
#include <ggl/geometries/register/point.hpp>
#include <ggl/strategies/strategies.hpp>
#include <ggl/util/write_dsv.hpp>

// Sample point, defining three color values
struct my_color_point
{
    double red, green, blue;
};

// Sample point, having an int array defined
struct my_array_point
{
    int c[3];
};

// Sample point, having x/y
struct my_2d
{
    float x,y;
};

// Sample class, protected and construction-time-only x/y,
// Can (of course) only used in algorithms which take const& points
class my_class_ro
{
public:
    my_class_ro(double x, double y) : m_x(x), m_y(y) {}
    double x() const { return m_x; }
    double y() const { return m_y; }
private:
    double m_x, m_y;
};

// Sample class using references for read/write
class my_class_rw
{
public:
    const double& x() const { return m_x; }
    const double& y() const { return m_y; }
    double& x() { return m_x; }
    double& y() { return m_y; }
private:
    double m_x, m_y;
};

// Sample class using getters / setters
class my_class_gs
{
public:
    const double get_x() const { return m_x; }
    const double get_y() const { return m_y; }
    void set_x(double v) { m_x = v; }
    void set_y(double v) { m_y = v; }
private:
    double m_x, m_y;
};

GEOMETRY_REGISTER_POINT_3D(my_color_point, double, cs::cartesian, red, green, blue)
GEOMETRY_REGISTER_POINT_3D(my_array_point, int, cs::cartesian, c[0], c[1], c[2])
GEOMETRY_REGISTER_POINT_2D(my_2d, float, cs::cartesian, x, y)
GEOMETRY_REGISTER_POINT_2D_CONST(my_class_ro, double, cs::cartesian, x(), y())
GEOMETRY_REGISTER_POINT_2D(my_class_rw, double, cs::cartesian, x(), y())
GEOMETRY_REGISTER_POINT_2D_GET_SET(my_class_gs, double, cs::cartesian, get_x, get_y, set_x, set_y)

int main()
{
    // Create 2 instances of our custom color point
    my_color_point c1 = ggl::make<my_color_point>(255, 3, 233);
    my_color_point c2 = ggl::make<my_color_point>(0, 50, 200);

    // The distance between them can be calculated using the cartesian method (=pythagoras)
    // provided with the library, configured by the coordinate_system type of the point
    std::cout << "color distance "
        << ggl::dsv(c1) << " to "
        << ggl::dsv(c2) << " is "
        << ggl::distance(c1,c2) << std::endl;

    my_array_point a1 = {{0}};
    my_array_point a2 = {{0}};
    ggl::assign(a1, 1, 2, 3);
    ggl::assign(a2, 3, 2, 1);

    std::cout << "color distance "
        << ggl::dsv(a1) << " to "
        << ggl::dsv(a2) << " is "
        << ggl::distance(a1,a2) << std::endl;

    my_2d p1 = {1, 5};
    my_2d p2 = {3, 4};
    std::cout << "float distance "
        << ggl::dsv(p1) << " to "
        << ggl::dsv(p2) << " is "
        << ggl::distance(p1,p2) << std::endl;

    my_class_ro cro1(1, 2);
    my_class_ro cro2(3, 4);
    std::cout << "class ro distance "
        << ggl::dsv(cro1) << " to "
        << ggl::dsv(cro2) << " is "
        << ggl::distance(cro1,cro2) << std::endl;

    my_class_rw crw1;
    my_class_rw crw2;
    ggl::assign(crw1, 1, 2);
    ggl::assign(crw2, 3, 4);
    std::cout << "class r/w distance "
        << ggl::dsv(crw1) << " to "
        << ggl::dsv(crw2) << " is "
        << ggl::distance(crw1,crw2) << std::endl;

    my_class_gs cgs1;
    my_class_gs cgs2;
    ggl::assign(cgs1, 1, 2);
    ggl::assign(cgs2, 3, 4);
    std::cout << "class g/s distance "
        << ggl::dsv(crw1) << " to "
        << ggl::dsv(crw2) << " is "
        << ggl::distance(cgs1,cgs2) << std::endl;

    return 0;
}
