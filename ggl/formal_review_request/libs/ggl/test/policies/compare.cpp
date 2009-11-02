// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <algorithm>

#include <ggl/algorithms/make.hpp>
#include <ggl/util/write_dsv.hpp>

#include <ggl/policies/compare.hpp>

#include <ggl/strategies/strategies.hpp>

#include <ggl/geometries/point.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>

#include <test_common/test_point.hpp>


template <typename Container>
inline std::string coordinates(Container const& points)
{
    std::ostringstream out;
    typedef typename boost::range_value<Container>::type point_type;
    for (typename boost::range_const_iterator<Container>::type it = boost::begin(points);
        it != boost::end(points);
        ++it)
    {
        out << ggl::dsv(*it);
    }
    return out.str();
}

template <typename P>
void test_2d()
{
    typedef typename ggl::coordinate_type<P>::type ct;

    std::vector<P> v;
    v.push_back(ggl::make<P>(3, 1));
    v.push_back(ggl::make<P>(2, 3));
    v.push_back(ggl::make<P>(2, 2));
    v.push_back(ggl::make<P>(1, 3));

    // Sort on coordinates in order x,y,z
    std::sort(v.begin(), v.end(), ggl::less<P>());
    std::string s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(1, 3)(2, 2)(2, 3)(3, 1)");

    // Reverse sort
    std::sort(v.begin(), v.end(), ggl::greater<P>());
    s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(3, 1)(2, 3)(2, 2)(1, 3)");

    // Sort backwards on coordinates in order x,y,z
    //std::sort(v.begin(), v.end(), ggl::greater<P>());
    //std::string s = coordinates(v);
    //BOOST_CHECK_EQUAL(s, "(1, 3)(2, 2)(2, 3)(3, 1)");

    // Refill to remove duplicate coordinates
    v.clear();
    v.push_back(ggl::make<P>(4, 1));
    v.push_back(ggl::make<P>(3, 2));
    v.push_back(ggl::make<P>(2, 3));
    v.push_back(ggl::make<P>(1, 4));

    // Sort ascending on only x-coordinate
    std::sort(v.begin(), v.end(), ggl::less<P, 0>());
    s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(1, 4)(2, 3)(3, 2)(4, 1)");

    // Sort ascending on only y-coordinate
    std::sort(v.begin(), v.end(), ggl::less<P, 1>());
    s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(4, 1)(3, 2)(2, 3)(1, 4)");

    // Sort descending on only x-coordinate
    std::sort(v.begin(), v.end(), ggl::greater<P, 0>());
    s = coordinates(v);
    //BOOST_CHECK_EQUAL(s, "(4, 1)(3, 2)(2, 3)(1, 4)");

    // Sort descending on only y-coordinate
    std::sort(v.begin(), v.end(), ggl::greater<P, 1>());
    s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(1, 4)(2, 3)(3, 2)(4, 1)");
}


template <typename P>
void test_spherical()
{
    typedef typename ggl::coordinate_type<P>::type ct;

    std::vector<P> v;
    v.push_back(ggl::make<P>( 179.73, 71.56)); // east
    v.push_back(ggl::make<P>( 177.47, 71.23)); // less east
    v.push_back(ggl::make<P>(-178.78, 70.78)); // further east, = west, this is the most right point

    // Sort on coordinates in order x,y,z
    std::sort(v.begin(), v.end(), ggl::less<P>());
    std::string s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(177.47, 71.23)(179.73, 71.56)(-178.78, 70.78)");

    // Sort ascending on only x-coordinate
    std::sort(v.begin(), v.end(), ggl::less<P, 0>());
    s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(177.47, 71.23)(179.73, 71.56)(-178.78, 70.78)");

    // Sort ascending on only x-coordinate, but override with std-comparison,
    // (so this is the normal sorting behaviour that would have been used
    // if it would not have been spherical)
    std::sort(v.begin(), v.end(), ggl::less<P, 0, std::less<ct> >());
    s = coordinates(v);
    BOOST_CHECK_EQUAL(s, "(-178.78, 70.78)(177.47, 71.23)(179.73, 71.56)");
}


int test_main(int, char* [])
{
    //test_2d<ggl::point<int, 2, ggl::cs::cartesian> >();
    //test_2d<ggl::point<float, 2, ggl::cs::cartesian> >();
    //test_2d<boost::tuple<double, double> >();
    test_2d<ggl::point<double, 2, ggl::cs::cartesian> >();

    test_spherical<ggl::point<double, 2, ggl::cs::spherical<ggl::degree> > >();

    return 0;
}
