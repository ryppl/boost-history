// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_INTERSECTION_HPP
#define GGL_TEST_INTERSECTION_HPP

#include <fstream>

#include <ggl_test_common.hpp>

#include <ggl/algorithms/intersection.hpp>
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
void test_intersection(int caseid, G1 const& g1, G2 const& g2,
        int expected_count = 0, int expected_point_count = 0,
        double expected_length_or_area = 0)
{
    static const bool is_line = ggl::geometry_id<OutputType>::type::value == 2;

    std::vector<OutputType> clip;
    ggl::intersection<OutputType>(g1, g2, std::back_inserter(clip));

    double length_or_area = 0;
    int n = 0;
    for (typename std::vector<OutputType>::const_iterator it = clip.begin();
            it != clip.end(); ++it)
    {
        // instead of specialization we check it run-time here
        length_or_area += is_line
            ? ggl::length(*it)
            : ggl::area(*it);

        // Simplify to get a correct point-count without duplicate points
        // (note that overlay might be adapted to avoid duplicates)
        OutputType simplified;
        ggl::simplify(*it, simplified, 0.0001);
        n += ggl::num_points(simplified);

        //std::cout << std::endl << "case " << caseid << " ";
        //std::cout << ggl::dsv(*it) << std::endl;
    }


    BOOST_CHECK_EQUAL(clip.size(), expected_count);
    BOOST_CHECK_EQUAL(n, expected_point_count);
    BOOST_CHECK_CLOSE(length_or_area, expected_length_or_area, 0.001);


#if defined(TEST_WITH_SVG)
    {
        std::ostringstream filename;
        filename << "intersection" << caseid << ".svg";

        std::ofstream svg(filename.str().c_str());

        svg_mapper<typename ggl::point_type<G2>::type> mapper(svg, 500, 500);
        mapper.add(g1);
        mapper.add(g2);

        mapper.map(g1, is_line
            ? "opacity:0.6;stroke:rgb(0,0,255);stroke-width:5"
            : "opacity:0.6;fill:rgb(0,0,255);stroke:rgb(0,0,0);stroke-width:1");
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
        int expected_count = 0, int expected_point_count = 0,
        double expected_length_or_area = 0)
{
    G1 g1;
    ggl::read_wkt(wkt1, g1);

    G2 g2;
    ggl::read_wkt(wkt2, g2);

    test_intersection<OutputType>(caseid, g1, g2,
        expected_count, expected_point_count,
        expected_length_or_area);
}



#endif
