// Generic Geometry Library
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/correct.hpp>

#include <ggl/strategies/strategies.hpp>

#include <ggl/multi/algorithms/correct.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>

#include <ggl/geometries/box.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/linestring.hpp>
#include <ggl/geometries/point_xy.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/polygon.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>


template <typename Geometry>
void test_geometry(std::string const& wkt, std::string const& expected)
{
    Geometry geometry;

    ggl::read_wkt(wkt, geometry);
    ggl::correct(geometry);

    std::ostringstream out;
    out << ggl::wkt(geometry);

    BOOST_CHECK_EQUAL(out.str(), expected);
}

template <typename P>
void test_all()
{
    typedef ggl::multi_polygon<ggl::polygon<P> > cw_type;
    std::string cw_mp = 
            "MULTIPOLYGON(((0 0,0 1,1 1,1 0,0 0)))";
    test_geometry<cw_type>(cw_mp, cw_mp);

    test_geometry<cw_type>("MULTIPOLYGON(((0 0,1 0,1 1,0 1,0 0)))", 
        cw_mp);
}

int test_main( int , char* [] )
{
    test_all<ggl::point_xy<double> >();

    return 0;
}
