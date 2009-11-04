// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>


#include <ggl_test_common.hpp>

#include <ggl/algorithms/make.hpp>
#include <ggl/algorithms/sectionalize.hpp>
#include <ggl/geometries/geometries.hpp>
#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>
#include <ggl/extensions/gis/io/wkt/write_wkt.hpp>

#include <test_common/test_point.hpp>

template <int DimensionCount, typename G>
void test_sectionalize(G const& g, int section_count,
        std::string const& index_check, std::string const& dir_check)
{
    typedef typename ggl::point_type<G>::type point;
    typedef ggl::box<point> box;
    typedef ggl::sections<box, DimensionCount> sections;

    sections s;
    ggl::sectionalize(g, s);

    BOOST_CHECK_EQUAL(s.size(), section_count);

    // Output streams for sections, boxes, other
    std::ostringstream out_sections;
    std::ostringstream out_boxes;
    std::ostringstream out_dirs;

    for (typename sections::size_type i = 0; i < s.size(); i++)
    {
        box const& b = s[i].bounding_box;

        if (i > 0)
        {
            out_sections << "|";
            out_dirs << "|";
            out_boxes << "|";
        }

        out_sections << s[i].begin_index << ".." << s[i].end_index;
        out_boxes << ggl::get<0,0>(b) << " " << ggl::get<0,1>(b)
            << ".." << ggl::get<1,0>(b) << " " << ggl::get<1,1>(b);
        for (int d = 0; d < DimensionCount; d++)
        {
            out_dirs << (d == 0 ? "" : " ");
            switch(s[i].directions[d])
            {
                case -99: out_dirs << "DUP"; break;
                case -1 : out_dirs << "-"; break;
                case  0 : out_dirs << "."; break;
                case +1 : out_dirs << "+"; break;
            }
        }
    }

    if (! index_check.empty())
    {
        BOOST_CHECK_EQUAL(out_sections.str(), index_check);
    }
    if (! dir_check.empty())
    {
        BOOST_CHECK_EQUAL(out_dirs.str(), dir_check);
    }
    else
    {
        std::cout << std::endl << ggl::wkt(g) << std::endl;
        std::cout << out_sections.str() << std::endl;
        //std::cout << out_boxes.str() << std::endl;
        std::cout << out_dirs.str() << std::endl;
    }
}

template <typename G>
void test_sectionalize(std::string const& wkt,
        int count2, std::string const& s2, std::string const d2,
        int count1, std::string const& s1, std::string const d1)
{
    G g;
    ggl::read_wkt(wkt, g);
    test_sectionalize<2>(g, count2, s2, d2);
    test_sectionalize<1>(g, count1, s1, d1);
}

template <typename P>
void test_all()
{
    test_sectionalize<ggl::linestring<P> >(
        "LINESTRING(1 1,2 2,3 0,5 0,5 8)",
        4, "0..1|1..2|2..3|3..4", "+ +|+ -|+ .|. +",
        2, "0..3|3..4", "+|.");

    test_sectionalize<ggl::polygon<P> >(
        "POLYGON((0 0,0 7,4 2,2 0,0 0))",
        4, "0..1|1..2|2..3|3..4", ". +|+ -|- -|- .",
        //            .   +   -   -   -> 3 sections
        3, "0..1|1..2|2..4", ".|+|-");

    test_sectionalize<ggl::polygon<P> >
        ("polygon((2.0 1.3, 2.4 1.7, 2.8 1.8, 3.4 1.2, 3.7 1.6,3.4 2.0, 4.1 3.0, 5.3 2.6, 5.4 1.2, 4.9 0.8, 2.9 0.7,2.0 1.3))",
        8, "0..2|2..3|3..4|4..5|5..6|6..8|8..10|10..11", "+ +|+ -|+ +|- +|+ +|+ -|- -|- +",
        4, "0..4|4..5|5..8|8..11", "+|-|+|-");


    test_sectionalize<ggl::polygon<P> >(
        "POLYGON((3 1,2 2,1 3,2 4,3 5,4 4,5 3,4 2,3 1))",
        4, "0..2|2..4|4..6|6..8", "- +|+ +|+ -|- -",
        //        -   -   -   +   +   +   +   -   - -> 3 sections
        3, "0..2|2..6|6..8", "-|+|-");

    // With holes
    test_sectionalize<ggl::polygon<P> >(
        "POLYGON((3 1,2 2,1 3,2 4,3 5,4 4,5 3,4 2,3 1), (3 2,2 2,3 4,3 2))",
        7, "0..2|2..4|4..6|6..8|0..1|1..2|2..3", "- +|+ +|+ -|- -|- .|+ +|. -",
        //        -   -   -   +   +   +   +   -   -          -   +   . -> 6 sections
        6, "0..2|2..6|6..8|0..1|1..2|2..3", "-|+|-|-|+|.");

    // With duplicates
    test_sectionalize<ggl::linestring<P> >(
        "LINESTRING(1 1,2 2,3 0,3 0,5 0,5 8)",
        5, "0..1|1..2|2..3|3..4|4..5", "+ +|+ -|DUP DUP|+ .|. +",
        4, "0..2|2..3|3..4|4..5", "+|DUP|+|.");
    // With two subsequent duplicate segments
    test_sectionalize<ggl::linestring<P> >(
        "LINESTRING(1 1,2 2,3 0,3 0,3 0,5 0,5 0,5 0,5 0,5 8)",
        6, "0..1|1..2|2..4|4..5|5..8|8..9", "+ +|+ -|DUP DUP|+ .|DUP DUP|. +",
        5, "0..2|2..4|4..5|5..8|8..9", "+|DUP|+|DUP|.");


    typedef ggl::box<P> B;
    test_sectionalize<2, B>(ggl::make<B>(0,0,4,4),
            4, "0..1|1..2|2..3|3..4", ". +|+ .|. -|- .");
    test_sectionalize<1, B>(ggl::make<B>(0,0,4,4),
            4, "0..1|1..2|2..3|3..4", ".|+|.|-");
}

int test_main(int, char* [])
{
    //test_all<ggl::point_xy<float> >();
    test_all<ggl::point_xy<double> >();

    return 0;
}
