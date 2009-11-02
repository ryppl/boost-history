// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Copyright Mateusz Loskot 2009, mateusz@loskot.net
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include <ggl_test_common.hpp>

#include <ggl/core/coordinate_type.hpp>
#include <ggl/geometries/cartesian2d.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/segment.hpp>
#include <ggl/iterators/segment_iterator.hpp>
#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>

template <typename C>
void test_linestring(const std::string& wkt, const std::string& expected)
{
    typedef C point_list;
    typedef typename C::value_type point;
    typedef typename C::iterator base_iterator;
    typedef ggl::segment_iterator<base_iterator, point> segment_iterator;
    typedef typename segment_iterator::value_type segment;
    typedef ggl::linestring<point> linestring;

    linestring g;
    ggl::read_wkt(wkt, g);

    point_list v;
    std::copy(g.begin(), g.end(), std::back_insert_iterator<point_list>(v));
    BOOST_CHECK_EQUAL(g.size(), v.size());

    segment_iterator it(v.begin(), v.end());
    segment_iterator end(v.end());

    std::ostringstream oss;
    while (it != end)
    {
        segment const& s = *it;
        oss << ggl::get<0>(s.first) << ggl::get<1>(s.first)
            << ggl::get<0>(s.second) << ggl::get<1>(s.second);
        ++it;
    }
    BOOST_CHECK_EQUAL(oss.str(), expected);
}

int test_main(int, char* [])
{
    // Test std::vector
    typedef std::vector<ggl::point_2d> points_v;
    test_linestring<points_v>("linestring empty", "");
    test_linestring<points_v>("linestring ()", "");
    test_linestring<points_v>("linestring (1 1)", "");
    test_linestring<points_v>("linestring (1 1, 2 2, 3 3)", "11222233");
    test_linestring<points_v>("linestring (1 1, 2 2, 3 3, 4 4)", "112222333344");
    test_linestring<points_v>("linestring (1 1, 2 2, 3 3, 4 4, 5 5, 6 6)", "11222233334444555566");

    // Test std::list
    typedef std::list<ggl::point_2d> points_l;
    test_linestring<points_l>("linestring empty", "");
    test_linestring<points_l>("linestring ()", "");
    test_linestring<points_l>("linestring (1 1)", "");
    test_linestring<points_l>("linestring (1 1, 2 2, 3 3)", "11222233");
    test_linestring<points_l>("linestring (1 1, 2 2, 3 3, 4 4)", "112222333344");
    test_linestring<points_l>("linestring (1 1, 2 2, 3 3, 4 4, 5 5, 6 6)", "11222233334444555566");

    return 0;
}
