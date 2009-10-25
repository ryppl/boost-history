// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <ggl_test_common.hpp>


#include <ggl/core/access.hpp>


#include <ggl/core/coordinate_type.hpp>
#include <ggl/algorithms/make.hpp>

#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>


#include <ggl/core/cs.hpp>
#include <ggl/geometries/point.hpp>
#include <ggl/geometries/segment.hpp>
#include <ggl/geometries/box.hpp>



template <typename G>
void test_get_set()
{
    typedef typename ggl::coordinate_type<G>::type coordinate_type;

    G g;
    ggl::set<0>(g, coordinate_type(1));
    ggl::set<1>(g, coordinate_type(2));

    coordinate_type x = ggl::get<0>(g);
    coordinate_type y = ggl::get<1>(g);

    BOOST_CHECK_CLOSE(double(x), 1.0, 0.0001);
    BOOST_CHECK_CLOSE(double(y), 2.0, 0.0001);
}

template <typename G>
void test_indexed_get_set(G& g)
{
    ggl::set<0, 0>(g, 1);
    ggl::set<0, 1>(g, 2);
    ggl::set<1, 0>(g, 3);
    ggl::set<1, 1>(g, 4);

    typedef typename ggl::coordinate_type<G>::type coordinate_type;
    coordinate_type x1 = ggl::get<0, 0>(g);
    coordinate_type y1 = ggl::get<0, 1>(g);
    coordinate_type x2 = ggl::get<1, 0>(g);
    coordinate_type y2 = ggl::get<1, 1>(g);

    BOOST_CHECK_CLOSE(double(x1), 1.0, 0.0001);
    BOOST_CHECK_CLOSE(double(y1), 2.0, 0.0001);
    BOOST_CHECK_CLOSE(double(x2), 3.0, 0.0001);
    BOOST_CHECK_CLOSE(double(y2), 4.0, 0.0001);
}

template <typename G, typename T>
void test_indexed_get(G const& g, T a, T b, T c, T d)
{
    T x1 = ggl::get<0, 0>(g);
    T y1 = ggl::get<0, 1>(g);
    T x2 = ggl::get<1, 0>(g);
    T y2 = ggl::get<1, 1>(g);

    BOOST_CHECK_CLOSE(double(x1), double(a), 0.0001);
    BOOST_CHECK_CLOSE(double(y1), double(b), 0.0001);
    BOOST_CHECK_CLOSE(double(x2), double(c), 0.0001);
    BOOST_CHECK_CLOSE(double(y2), double(d), 0.0001);
}

template <typename P>
void test_all()
{
    typedef typename ggl::coordinate_type<P>::type coordinate_type;

    // POINT, setting coordinate
    test_get_set<P>();


    // BOX, setting left/right/top/bottom
    ggl::box<P> b;
    test_indexed_get_set(b);

    // SEGMENT (in GGL not having default constructor; however that is not a requirement)
    P p1 = ggl::make_zero<P>();
    P p2 = ggl::make_zero<P>();
    ggl::segment<P> s(p1, p2);
    test_indexed_get_set(s);

    // CONST SEGMENT
    ggl::set<0>(p1, 1); // we don't use assign because dim in {2,3}
    ggl::set<1>(p1, 2);
    ggl::set<0>(p2, 3);
    ggl::set<1>(p2, 4);
    ggl::segment<const P> cs(p1, p2);
    test_indexed_get(cs,
        coordinate_type(1), coordinate_type(2),
        coordinate_type(3), coordinate_type(4));
}


int test_main(int, char* [])
{
    test_get_set<int[2]>();
    test_get_set<float[2]>();
    test_get_set<double[2]>();
    test_get_set<double[3]>();

    test_get_set<boost::tuple<double, double> >();

    test_all<ggl::point<int, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<float, 2, ggl::cs::cartesian> >();
    test_all<ggl::point<double, 2, ggl::cs::cartesian> >();

    return 0;
}
