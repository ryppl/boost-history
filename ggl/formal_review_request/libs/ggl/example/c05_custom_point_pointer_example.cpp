// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Custom pointer-to-point example

#include <iostream>

#include <ggl/algorithms/distance.hpp>
#include <ggl/algorithms/length.hpp>
#include <ggl/algorithms/make.hpp>

#include <ggl/algorithms/intersection_linestring.hpp>
#include <ggl/algorithms/intersection.hpp>

#include <ggl/geometries/cartesian2d.hpp>

#include <ggl/geometries/adapted/std_as_linestring.hpp>

// Sample point, having x/y
struct my_point
{
    double x,y;
};


namespace ggl { namespace traits {

template<> struct tag<my_point*>
{ typedef point_tag type; };

template<> struct coordinate_type<my_point*>
{ typedef double type; };

template<> struct coordinate_system<my_point*>
{ typedef cs::cartesian type; };

template<> struct dimension<my_point*> : boost::mpl::int_<2> {};

template<>
struct access<my_point*, 0>
{
    static double get(my_point const* p)
    {
        return p->x;
    }

    static void set(my_point* p, double const& value)
    {
        p->x = value;
    }
};

template<>
struct access<my_point*, 1>
{
    static double get(my_point const* p)
    {
        return p->y;
    }

    static void set(my_point* p, double const& value)
    {
        p->y = value;
    }
};

}} // namespace ggl::traits



int main()
{
    typedef std::vector<my_point*> ln;
    ln myline;
    for (float i = 0.0; i < 10.0; i++)
    {
        my_point* p = new my_point;
        p->x = i;
        p->y = i + 1;
        myline.push_back(p);
    }

    std::cout << ggl::length(myline) << std::endl;

    ggl::box_2d cb(ggl::point_2d(1.5, 1.5), ggl::point_2d(4.5, 4.5));

    // This will NOT work because would need dynamicly allocating memory for point* in algorithms:
    // std::vector<ln> clipped;
    //ggl::intersection(cb, myline, std::back_inserter(clipped));

    // This works because outputs to a normal struct point, no point*
    std::vector<ggl::linestring_2d> clipped;
    ggl::strategy::intersection::liang_barsky<ggl::box_2d, ggl::point_2d> strategy;
    ggl::detail::intersection::clip_linestring_with_box<ggl::linestring_2d>(cb,
                    myline, std::back_inserter(clipped), strategy);


    std::cout << ggl::length(clipped.front()) << std::endl;

    // free
    for (unsigned int i = 0; i < myline.size(); i++)
    {
        delete myline[i];
    }



    return 0;
}
