// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>


#include <ggl/algorithms/assign.hpp>
#include <ggl/algorithms/convert.hpp>
#include <ggl/algorithms/make.hpp>
#include <ggl/algorithms/num_points.hpp>

#include <ggl/geometries/geometries.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>
#include <test_common/test_point.hpp>

template <typename P>
void test_all()
{
    typedef ggl::box<P> box_type;

    P p;
    ggl::assign(p, 1, 2);

    box_type b;
    ggl::convert(p, b);

    BOOST_CHECK_CLOSE(double(ggl::get<0, 0>(b)), 1.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<0, 1>(b)), 2.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1, 0>(b)), 1.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1, 1>(b)), 2.0, 0.001);
}

template <typename P>
void test_std()
{
    test_all<P>();

    typedef ggl::box<P> box_type;
    typedef ggl::linear_ring<P> ring_type;
    typedef ggl::polygon<P> polygon_type;

    box_type b;
    ggl::set<ggl::min_corner, 0>(b, 1);
    ggl::set<ggl::min_corner, 1>(b, 2);
    ggl::set<ggl::max_corner, 0>(b, 3);
    ggl::set<ggl::max_corner, 1>(b, 4);

    ring_type ring;
    ggl::convert(b, ring);

    //std::cout << ggl::wkt(b) << std::endl;
    //std::cout << ggl::wkt(ring) << std::endl;

    typename boost::range_const_iterator<ring_type>::type it = ring.begin();
    BOOST_CHECK_CLOSE(double(ggl::get<0>(*it)), 1.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1>(*it)), 2.0, 0.001);
    it++;
    BOOST_CHECK_CLOSE(double(ggl::get<0>(*it)), 1.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1>(*it)), 4.0, 0.001);
    it++;
    BOOST_CHECK_CLOSE(double(ggl::get<0>(*it)), 3.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1>(*it)), 4.0, 0.001);
    it++;
    BOOST_CHECK_CLOSE(double(ggl::get<0>(*it)), 3.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1>(*it)), 2.0, 0.001);
    it++;
    BOOST_CHECK_CLOSE(double(ggl::get<0>(*it)), 1.0, 0.001);
    BOOST_CHECK_CLOSE(double(ggl::get<1>(*it)), 2.0, 0.001);

    BOOST_CHECK_EQUAL(ring.size(), 5);


    polygon_type polygon;

    ggl::convert(ring, polygon);
    BOOST_CHECK_EQUAL(ggl::num_points(polygon), 5);

    ggl::convert(polygon, ring);
    BOOST_CHECK_EQUAL(ggl::num_points(ring), 5);
}

int test_main(int, char* [])
{
    test_std<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_std<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_std<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
