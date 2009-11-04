// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_ENVELOPE_HPP
#define GGL_TEST_ENVELOPE_HPP


#include <ggl_test_common.hpp>

#include <ggl/algorithms/envelope.hpp>
#include <ggl/geometries/box.hpp>
#include <ggl/strategies/strategies.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>


template<std::size_t DimensionCount>
struct check_result
{};

template <>
struct check_result<2>
{
    template <typename Box, typename T>
    static void apply(Box const& b, const T& x1, const T& y1, const T& z1,
                const T& x2, const T& y2, const T& z2)
    {
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::min_corner, 0>(b)), double(x1), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::min_corner, 1>(b)), double(y1), 0.001);

        BOOST_CHECK_CLOSE(double(ggl::get<ggl::max_corner, 0>(b)), double(x2), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::max_corner, 1>(b)), double(y2), 0.001);
    }
};

template <>
struct check_result<3>
{
    template <typename Box, typename T>
    static void apply(Box const& b, const T& x1, const T& y1, const T& z1,
                const T& x2, const T& y2, const T& z2)
    {
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::min_corner, 0>(b)), double(x1), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::min_corner, 1>(b)), double(y1), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::min_corner, 2>(b)), double(z1), 0.001);

        BOOST_CHECK_CLOSE(double(ggl::get<ggl::max_corner, 0>(b)), double(x2), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::max_corner, 1>(b)), double(y2), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<ggl::max_corner, 2>(b)), double(z2), 0.001);
    }
};


template <typename Geometry, typename T>
void test_envelope(std::string const& wkt,
                   const T& x1, const T& x2,
                   const T& y1, const T& y2,
                   const T& z1 = 0, const T& z2 = 0)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);
    ggl::box<typename ggl::point_type<Geometry>::type > b;
    ggl::envelope(geometry, b);

    check_result<ggl::dimension<Geometry>::type::value>::apply(b, x1, y1, z1, x2, y2, z2);
}

template <typename Geometry, typename T>
void test_envelope_strategy(std::string const& wkt,
                   const T& x1, const T& x2,
                   const T& y1, const T& y2,
                   const T& z1 = 0, const T& z2 = 0)
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);
    ggl::box<typename ggl::point_type<Geometry>::type > b;
    ggl::envelope(geometry, b);

    check_result<ggl::dimension<Geometry>::type::value>::apply(b, x1, y1, z1, x2, y2, z2);
}



#endif
