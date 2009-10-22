// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <ggl_test_common.hpp>

#include <ggl/algorithms/length.hpp>

#include <ggl/extensions/gis/io/wkt/read_wkt.hpp>

#include <ggl/geometries/geometries.hpp>

#include <algorithms/test_length.hpp>


namespace ggl { namespace traits {

template <typename Point>
struct tag<std::pair<Point, Point> >
{
    typedef segment_tag type;
};

template <typename Point>
struct point_type<std::pair<Point, Point> >
{
    typedef Point type;
};

template <typename Point, std::size_t Dimension>
struct indexed_access<std::pair<Point, Point>, 0, Dimension>
{
    typedef typename ggl::coordinate_type<Point>::type coordinate_type;

    static inline coordinate_type get(std::pair<Point, Point> const& s)
    {
        return ggl::get<Dimension>(s.first);
    }

    static inline void set(std::pair<Point, Point>& s, coordinate_type const& value)
    {
        ggl::set<Dimension>(s.first, value);
    }
};


template <typename Point, std::size_t Dimension>
struct indexed_access<std::pair<Point, Point>, 1, Dimension>
{
    typedef typename ggl::coordinate_type<Point>::type coordinate_type;

    static inline coordinate_type get(std::pair<Point, Point> const& s)
    {
        return ggl::get<Dimension>(s.second);
    }

    static inline void set(std::pair<Point, Point>& s, coordinate_type const& value)
    {
        ggl::set<Dimension>(s.second, value);
    }
};

}} // namespace traits



template <typename P>
void test_all()
{
    // 3-4-5 triangle
    //test_geometry<std::pair<P, P> >("LINESTRING(0 0,3 4)", 5);


    // 3-4-5 plus 1-1
    test_geometry<ggl::linestring<P> >("LINESTRING(0 0,3 4,4 3)", 5 + sqrt(2.0));

    // Geometries with length zero
    test_geometry<P>("POINT(0 0)", 0);
    test_geometry<ggl::polygon<P> >("POLYGON((0 0,0 1,1 1,1 0,0 0))", 0);
}

int test_main(int, char* [])
{
    test_all<ggl::point_xy<int> >();
    test_all<ggl::point_xy<float> >();
    test_all<ggl::point_xy<double> >();

#if defined(HAVE_CLN)
    test_all<ggl::point_xy<boost::numeric_adaptor::cln_value_type> >();
#endif
#if defined(HAVE_GMP)
    test_all<ggl::point_xy<boost::numeric_adaptor::gmp_value_type> >();
#endif

    return 0;
}
