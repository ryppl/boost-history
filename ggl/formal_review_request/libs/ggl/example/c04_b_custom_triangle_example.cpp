// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Custom triangle template Example

#include <iostream>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/centroid.hpp>
#include <ggl/geometries/adapted/tuple.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>

#include <ggl/util/write_dsv.hpp>

#include <ggl/geometries/register/ring.hpp>


template <typename P>
struct triangle : public boost::array<P, 3>
{
};

// Register triangle<P>
GEOMETRY_REGISTER_RING_TEMPLATIZED(triangle)


namespace ggl { namespace dispatch {

// Specializations of area dispatch structure, implement algorithm
template<typename P, typename S>
struct area<ring_tag, triangle<P>, S>
{
    static inline double apply(triangle<P> const& t, S const&)
    {
        return 0.5  * ((get<0>(t[1]) - get<0>(t[0])) * (get<1>(t[2]) - get<1>(t[0]))
                     - (get<0>(t[2]) - get<0>(t[0])) * (get<1>(t[1]) - get<1>(t[0])));
    }
};

}} // namespace ggl::dispatch


int main()
{
    //triangle<ggl::point_xy<double> > t;
    triangle<boost::tuple<double, double> > t;
    t[0] = boost::make_tuple(0, 0);
    t[1] = boost::make_tuple(5, 0);
    t[2] = boost::make_tuple(2.5, 2.5);

    std::cout << "Triangle: " << ggl::dsv(t) << std::endl;
    std::cout << "Area: " << ggl::area(t) << std::endl;

    //ggl::point_xy<double> c;
    boost::tuple<double, double> c;
    ggl::centroid(t, c);
    std::cout << "Centroid: " << ggl::dsv(c) << std::endl;

    return 0;
}
