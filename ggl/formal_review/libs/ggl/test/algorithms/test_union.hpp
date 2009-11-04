// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_UNION_HPP
#define GGL_TEST_UNION_HPP

#include <fstream>

#include <ggl_test_common.hpp>

#include <ggl/algorithms/union.hpp>

#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/length.hpp>
#include <ggl/algorithms/num_points.hpp>
#include <ggl/algorithms/simplify.hpp>

#include <ggl/geometries/geometries.hpp>

#include <ggl/strategies/strategies.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>


#if defined(TEST_WITH_SVG)
#  include <test_util/svg_mapper.hpp>
#endif



template <typename OutputType, typename G1, typename G2>
void test_union(int caseid, G1 const& g1, G2 const& g2,
        int expected_count, int expected_hole_count,
        int expected_point_count, double expected_area)
{
    std::vector<OutputType> clip;
    ggl::union_inserter<OutputType>(g1, g2, std::back_inserter(clip));

    double area = 0;
    int n = 0;
    int holes = 0;
    for (typename std::vector<OutputType>::const_iterator it = clip.begin();
            it != clip.end(); ++it)
    {
        area += ggl::area(*it);
        holes += ggl::num_interior_rings(*it);

        // Simplify to get a correct point-count without duplicate points
        // (note that overlay might be adapted to avoid duplicates)
        OutputType simplified;
        ggl::simplify(*it, simplified, 0.0001);
        n += ggl::num_points(simplified);
    }


    /***
    std::cout << "case: " << caseid
        << " n: " << n
        << " area: " << area
        << " polygons: " << boost::size(clip)
        << " holes: " << holes
        << std::endl;
    ***/

    BOOST_CHECK_EQUAL(clip.size(), expected_count);
    BOOST_CHECK_EQUAL(n, expected_point_count);
    BOOST_CHECK_EQUAL(holes, expected_hole_count);
    BOOST_CHECK_CLOSE(area, expected_area, 0.001);

#if defined(TEST_WITH_SVG)
    {
        std::ostringstream filename;
        filename << "union" << caseid << ".svg";

        std::ofstream svg(filename.str().c_str());

        svg_mapper<typename ggl::point_type<G2>::type> mapper(svg, 500, 500);
        mapper.add(g1);
        mapper.add(g2);

        mapper.map(g1, "opacity:0.6;fill:rgb(0,0,255);stroke:rgb(0,0,0);stroke-width:1");
        mapper.map(g2, "opacity:0.6;fill:rgb(0,255,0);stroke:rgb(0,0,0);stroke-width:1");

        for (typename std::vector<OutputType>::const_iterator it = clip.begin();
                it != clip.end(); ++it)
        {
            mapper.map(*it, "opacity:0.6;fill:none;stroke:rgb(255,0,0);stroke-width:5");
        }
    }
#endif
}

template <typename OutputType, typename G1, typename G2>
void test_one(int caseid, std::string const& wkt1, std::string const& wkt2,
        int expected_count, int expected_hole_count,
        int expected_point_count, double expected_area)
{
    G1 g1;
    ggl::read_wkt(wkt1, g1);

    G2 g2;
    ggl::read_wkt(wkt2, g2);

    test_union<OutputType>(caseid, g1, g2,
        expected_count, expected_hole_count, expected_point_count,
        expected_area);
}



#endif
