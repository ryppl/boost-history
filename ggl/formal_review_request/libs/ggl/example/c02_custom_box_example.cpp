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

#include <ggl/algorithms/make.hpp>
#include <ggl/algorithms/within.hpp>
#include <ggl/geometries/register/point.hpp>
#include <ggl/geometries/register/box.hpp>

#include <ggl/util/write_dsv.hpp>

struct my_point
{
    double x, y;
};

struct my_int_point
{
    int x, y;
};

struct my_box
{
    my_point ll, ur;
};

struct my_box_ltrb
{
    int left, top, right, bottom;
};

struct my_box_4
{
    double coors[4];
};

template <typename P>
struct my_box_t
{
    P ll, ur;
};

GEOMETRY_REGISTER_POINT_2D(my_point, double, cs::cartesian, x, y)
GEOMETRY_REGISTER_POINT_2D(my_int_point, int, cs::cartesian, x, y)
GEOMETRY_REGISTER_BOX(my_box, my_point, ll, ur)
GEOMETRY_REGISTER_BOX_TEMPLATIZED(my_box_t, ll, ur)
GEOMETRY_REGISTER_BOX_2D_4VALUES(my_box_ltrb, my_int_point, left, top, right, bottom)
GEOMETRY_REGISTER_BOX_2D_4VALUES(my_box_4, my_point, coors[0], coors[1], coors[2], coors[3])

int main()
{
    my_point p = ggl::make<my_point>(3.5, 3.5);
    my_box b = ggl::make<my_box>(0, 0, 2, 2);
    my_box_ltrb b1 = ggl::make<my_box_ltrb>(0, 0, 3, 3);
    my_box_4 b4 = ggl::make<my_box_4>(0, 0, 4, 4);
    my_box_t<my_point> bt = ggl::make<my_box_t<my_point> >(0, 0, 5, 5);

    std::cout << ggl::dsv(p) << " IN " << ggl::dsv(b) 
            << " : " << int(ggl::within(p, b)) << std::endl;
    std::cout << ggl::dsv(p) << " IN " << ggl::dsv(b1) 
            << " : " << int(ggl::within(p, b1)) << std::endl;
    std::cout << ggl::dsv(p) << " IN " << ggl::dsv(b4) 
            << " : " << int(ggl::within(p, b4)) << std::endl;
    std::cout << ggl::dsv(p) << " IN " << ggl::dsv(bt) 
            << " : " << int(ggl::within(p, bt)) << std::endl;

    return 0;
}
