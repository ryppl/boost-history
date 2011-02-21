// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2011, Geodan, Amsterdam, the Netherlands
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Quickbook Example

//[make_3d_point
//` Using make to construct a three dimensional point

#include <iostream>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

int main()
{
    typedef boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> point_type;
    point_type p = boost::geometry::make<point_type>(1, 2, 3);
    std::cout << boost::geometry::dsv(p) << std::endl;
    return 0;
}

//]


//[make_3d_point_output
/*`
Output:
[pre
(1, 2, 3)
]
*/
//]
