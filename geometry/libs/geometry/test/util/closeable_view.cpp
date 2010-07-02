// Boost.Geometry (aka GGL, Generic Geometry Library) test file
//
// Copyright Barend Gehrels 2010, Geodan, Amsterdam, the Netherlands
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

#include <geometry_test_common.hpp>

#include <boost/geometry/util/closeable_view.hpp>

#include <boost/geometry/extensions/gis/io/wkt/read_wkt.hpp>
#include <boost/geometry/util/write_dsv.hpp>
#include <boost/geometry/geometries/cartesian2d.hpp>
#include <boost/geometry/geometries/adapted/tuple_cartesian.hpp>


template <bool Close, typename Range>
void test_optionally_closing(Range const& range, std::string const& expected)
{
    typedef boost::geometry::closeable_view<Range const, Close> view_type;
    typedef typename boost::range_iterator<view_type const>::type iterator;

    view_type view(range);

    bool first = true;
    std::ostringstream out;
    iterator end = boost::end(view);
    for (iterator it = boost::begin(view); it != end; ++it, first = false)
    {
        out << (first ? "" : " ") << boost::geometry::dsv(*it);
    }
    BOOST_CHECK_EQUAL(out.str(), expected);
}


template <typename Geometry>
void test_geometry(std::string const& wkt, 
            std::string const& expected_false, 
            std::string const& expected_true)
{
    namespace bg = boost::geometry;
    Geometry geo;
    bg::read_wkt(wkt, geo);

    test_optionally_closing<false>(geo, expected_false);
    test_optionally_closing<true>(geo, expected_true);
}


template <typename P>
void test_all()
{
    test_geometry<boost::geometry::linear_ring<P> >(
            "POLYGON((1 1,1 4,4 4,4 1))", 
            "(1, 1) (1, 4) (4, 4) (4, 1)",
            "(1, 1) (1, 4) (4, 4) (4, 1) (1, 1)");
}


int test_main(int, char* [])
{
    namespace bg = boost::geometry;
    test_all<bg::point_2d>();
    test_all<bg::point<int, 2, bg::cs::cartesian> >();
    test_all<boost::tuple<double, double> >();

    return 0;
}