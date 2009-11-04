// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_COMBINE_HPP
#define GGL_TEST_COMBINE_HPP


#include <ggl_test_common.hpp>

#include <ggl/algorithms/combine.hpp>
#include <ggl/strategies/strategies.hpp>
#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>
#include <ggl/algorithms/assign.hpp>
#include <ggl/util/write_dsv.hpp>


template <typename Geometry, typename Box>
void test_combine(Box& box,
                  std::string const& wkt,
                  std::string const& expected)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);

    ggl::combine(box, geometry);

    std::ostringstream out;
    out << ggl::dsv(box, ",", "(", ")", ",", "", "");

    BOOST_CHECK_EQUAL(out.str(), expected);
}

template <typename Geometry, typename Box>
void test_combine_other_strategy(Box& box,
                  std::string const& wkt,
                  std::string const& expected)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);

    
    ggl::combine(box, geometry);

    std::ostringstream out;
    out << ggl::dsv(box, ",", "(", ")", ",", "", "");

    BOOST_CHECK_EQUAL(out.str(), expected);
}


#endif
