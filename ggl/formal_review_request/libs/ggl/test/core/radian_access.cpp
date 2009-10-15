// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/included/test_exec_monitor.hpp>


#include <ggl/core/coordinate_type.hpp>
#include <ggl/core/radian_access.hpp>

#include <ggl/core/cs.hpp>
#include <ggl/geometries/point.hpp>


template <typename P, typename T>
void test_get(T const& c1, T const& c2,
    T const& e1, T const& e2)
{
    typedef typename ggl::coordinate_type<P>::type coordinate_type;

    P p;
    ggl::set<0>(p, coordinate_type(c1));
    ggl::set<1>(p, coordinate_type(c2));

    coordinate_type g1 = ggl::get_as_radian<0>(p);
    coordinate_type g2 = ggl::get_as_radian<1>(p);

    BOOST_CHECK_CLOSE(double(g1), double(e1), 0.0001);
    BOOST_CHECK_CLOSE(double(g2), double(e2), 0.0001);
}


template <typename P, typename T>
void test_set(T const& c1, T const& c2,
    T const& e1, T const& e2)
{
    typedef typename ggl::coordinate_type<P>::type coordinate_type;

    P p;
    ggl::set_from_radian<0>(p, coordinate_type(c1));
    ggl::set_from_radian<1>(p, coordinate_type(c2));

    coordinate_type g1 = ggl::get<0>(p);
    coordinate_type g2 = ggl::get<1>(p);

    BOOST_CHECK_CLOSE(double(g1), double(e1), 0.0001);
    BOOST_CHECK_CLOSE(double(g2), double(e2), 0.0001);
}


template <typename T>
void test()
{
    double d2r = 3.1415926535897932384626433832795 / 180.0;

    test_get<ggl::point<T, 2, ggl::cs::spherical<ggl::degree> > >
        (1.0, 2.0, 1.0 * d2r, 2.0 * d2r);

    test_get<ggl::point<T, 2, ggl::cs::spherical<ggl::radian> > >
        (1.0, 2.0, 1.0, 2.0);

    test_set<ggl::point<T, 2, ggl::cs::spherical<ggl::radian> > >
        (1.0, 2.0, 1.0, 2.0);
    test_set<ggl::point<T, 2, ggl::cs::spherical<ggl::degree> > >
        (1.0 * d2r, 2.0 * d2r, 1.0, 2.0);

}


int test_main(int, char* [])
{
    //test<float>();
    test<double>();

    return 0;
}
