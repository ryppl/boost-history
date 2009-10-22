// Generic Geometry Library Point concept test file
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "function_requiring_a_point.hpp"

#include <ggl/core/cs.hpp>

struct point
{
    float x, y;
};

namespace ggl { namespace traits {

template <> struct tag<point> { typedef point_tag type; };
template <> struct coordinate_type<point> { typedef float type; };
template <> struct coordinate_system<point> { typedef ggl::cs::cartesian type; };
template <> struct dimension<point> { enum { value = 2 }; };

template <> struct access<point, 0>
{
    static float get(point const& p) { return p.x; }
};

template <> struct access<point, 1>
{
    static float get(point const& p) { return p.y; }
};


}} // namespace ggl::traits

int main()
{
    point p1;
    const point p2;
    ggl::function_requiring_a_point(p1, p2);
}
