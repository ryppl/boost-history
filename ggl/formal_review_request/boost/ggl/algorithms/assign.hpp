// Generic Geometry Library
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_ASSIGN_HPP
#define GGL_ALGORITHMS_ASSIGN_HPP

#include <cstddef>

#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <ggl/algorithms/append.hpp>
#include <ggl/algorithms/clear.hpp>
#include <ggl/core/access.hpp>
#include <ggl/core/exterior_ring.hpp>
#include <ggl/core/tags.hpp>

#include <ggl/geometries/concepts/check.hpp>


#include <ggl/util/copy.hpp>
#include <ggl/util/for_each_coordinate.hpp>

/*!
\defgroup access access: get/set coordinate values, make objects, clear geometries, append point(s)
\details There are many ways to edit geometries. It is possible to:
- use the geometries themselves, so access point.x(). This is not done inside the library because it is agnostic
 to geometry type. However, library users can use this as it is intuitive.
- use the standard library, so use .push_back(point) or use inserters. This is also avoided inside the library.
However, library users can use it if they are used to the standard library
- use the functionality provided in this geometry library. These are the functions in this module.

The library provides the following functions to edit geometries:
- set to set one coordinate value
- assign to set two or more coordinate values
- make to construct and return geometries with specified coordinates.
- append to append one or more points to a geometry
- clear to remove all points from a geometry

For getting coordinates it is similar:
- get to get a coordinate value
- or use the standard library
- or use the geometries themselves

*/

namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace assign {

template <typename C>
struct assign_operation
{
    inline assign_operation(C const& value)
        : m_value(value)
    {}

    template <typename P, std::size_t I>
    inline void apply(P& point) const
    {
        set<I>(point, m_value);
    }

private:

    C m_value;
};


/*!
    \brief Assigns all coordinates of a specific point to a value
    \ingroup access
    \details
    \param p Point
    \param value Value which is assigned to all coordinates of point p
 */
template <typename P>
inline void assign_value(P& p, typename coordinate_type<P>::type const& value)
{
    for_each_coordinate(p,
            assign_operation<typename coordinate_type<P>::type>(value));
}



template <typename B, std::size_t C, std::size_t I, std::size_t D>
struct initialize
{
    typedef typename coordinate_type<B>::type coordinate_type;

    static inline void apply(B& box, const coordinate_type& value)
    {
        set<C, I>(box, value);
        initialize<B, C, I + 1, D>::apply(box, value);
    }
};

template <typename B, std::size_t C, std::size_t D>
struct initialize<B, C, D, D>
{
    typedef typename coordinate_type<B>::type coordinate_type;

    static inline void apply(B& box, const coordinate_type& value)
    {
        boost::ignore_unused_variable_warning(box);
        boost::ignore_unused_variable_warning(value);
    }
};

template <typename Point>
struct assign_zero_point
{
    static inline void apply(Point& point)
    {
        typedef typename coordinate_type<Point>::type coordinate_type;
        assign_value(point, 0);
    }
};


template <typename Box>
struct assign_inverse_box
{
    typedef typename point_type<Box>::type point_type;

    static inline void apply(Box& box)
    {
        typedef typename coordinate_type<point_type>::type coordinate_type;

        initialize
            <
                Box, min_corner, 0, dimension<Box>::type::value
            >::apply(
            box, boost::numeric::bounds<coordinate_type>::highest());
        initialize
            <
                Box, max_corner, 0, dimension<Box>::type::value
            >::apply(
            box, boost::numeric::bounds<coordinate_type>::lowest());
    }
};

template <typename Box>
struct assign_zero_box
{
    static inline void apply(Box& box)
    {
        typedef typename coordinate_type<Box>::type coordinate_type;

        initialize
            <
                Box, min_corner, 0, dimension<Box>::type::value
            >::apply(box, coordinate_type());
        initialize
            <
                Box, max_corner, 0, dimension<Box>::type::value
            >::apply(box, coordinate_type());
    }
};




template <std::size_t C1, std::size_t C2, typename Box, typename Point>
inline void assign_box_2d_corner(Box const& box, Point& point)
{
    // Be sure both are 2-Dimensional
    assert_dimension<Box, 2>();
    assert_dimension<Point, 2>();

    // Copy coordinates
    typedef typename coordinate_type<Point>::type coordinate_type;

    set<0>(point, boost::numeric_cast<coordinate_type>(get<C1, 0>(box)));
    set<1>(point, boost::numeric_cast<coordinate_type>(get<C2, 1>(box)));
}





template
<
    typename Box, typename Point,
    std::size_t Corner,
    std::size_t Dimension, std::size_t Count
>
struct assign_point_to_box
{
    typedef typename coordinate_type<Box>::type coordinate_type;

    static inline void apply(Point const& point, Box& box)
    {
        ggl::set<Corner, Dimension>
            (
                box,
                boost::numeric_cast<coordinate_type>(ggl::get<Dimension>(point))
            );

        assign_point_to_box
            <
                Box, Point, Corner, Dimension + 1, Count
            >::apply(point, box);
    }
};

template
<
    typename Box, typename Point,
    std::size_t Corner,
    std::size_t Count
>
struct assign_point_to_box<Box, Point, Corner, Count, Count>
{
    static inline void apply(Point const& point, Box& box)
    {
    }
};



}} // namespace detail::assign
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename GeometryTag, typename Geometry, std::size_t DimensionCount>
struct assign {};

template <typename P>
struct assign<point_tag, P, 2>
{
    typedef typename coordinate_type<P>::type coordinate_type;

    template <typename T>
    static inline void apply(P& point, T const& c1, T const& c2)
    {
        set<0>(point, boost::numeric_cast<coordinate_type>(c1));
        set<1>(point, boost::numeric_cast<coordinate_type>(c2));
    }
};

template <typename P>
struct assign<point_tag, P, 3>
{
    typedef typename coordinate_type<P>::type coordinate_type;

    template <typename T>
    static inline void apply(P& point, T const& c1, T const& c2, T const& c3)
    {
        set<0>(point, boost::numeric_cast<coordinate_type>(c1));
        set<1>(point, boost::numeric_cast<coordinate_type>(c2));
        set<2>(point, boost::numeric_cast<coordinate_type>(c3));
    }
};

template <typename B>
struct assign<box_tag, B, 2>
{
    typedef typename coordinate_type<B>::type coordinate_type;

    // Here we assign 4 coordinates to a box.
    // -> Most logical is: x1,y1,x2,y2
    // In case the user reverses x1/x2 or y1/y2, we could reverse them (THAT IS NOT IMPLEMENTED)

    // Note also comment in util/assign_box_corner ->
    //   ("Most logical is LOWER, UPPER and sub-order LEFT, RIGHT")
    //   (That is assigning 4 points from a box. So lower-left, lower-right, upper-left, upper-right)
    template <typename T>
    static inline void apply(B& box, T const& x1, T const& y1, T const& x2, T const& y2)
    {
        set<min_corner, 0>(box, boost::numeric_cast<coordinate_type>(x1));
        set<min_corner, 1>(box, boost::numeric_cast<coordinate_type>(y1));
        set<max_corner, 0>(box, boost::numeric_cast<coordinate_type>(x2));
        set<max_corner, 1>(box, boost::numeric_cast<coordinate_type>(y2));
    }
};



template <typename GeometryTag, typename Geometry>
struct assign_zero {};


template <typename Point>
struct assign_zero<point_tag, Point>
    : detail::assign::assign_zero_point<Point>
{};

template <typename Box>
struct assign_zero<box_tag, Box>
    : detail::assign::assign_zero_box<Box>
{};


template <typename GeometryTag, typename Geometry>
struct assign_inverse {};

template <typename Box>
struct assign_inverse<box_tag, Box>
    : detail::assign::assign_inverse_box<Box>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
    \brief assign two values to a 2D point
    \ingroup access
 */
template <typename Geometry, typename T>
inline void assign(Geometry& geometry, T const& c1, T const& c2)
{
    concept::check<Geometry>();

    dispatch::assign
        <
            typename tag<Geometry>::type,
            Geometry,
            ggl::dimension<Geometry>::type::value
        >::apply(geometry, c1, c2);
}

/*!
    \brief assign three values to a 3D point [or the center + radius to a circle]
    \ingroup access
 */
template <typename Geometry, typename T>
inline void assign(Geometry& geometry, T const& c1, T const& c2, T const& c3)
{
    concept::check<Geometry>();

    dispatch::assign
        <
            typename tag<Geometry>::type,
            Geometry,
            ggl::dimension<Geometry>::type::value
        >::apply(geometry, c1, c2, c3);
}

/*!
    \brief assign center + radius to a sphere [for extension]
    \ingroup access
 */
template <typename Geometry, typename T>
inline void assign(Geometry& geometry, T const& c1, T const& c2, T const& c3, T const& c4)
{
    concept::check<Geometry>();

    dispatch::assign
        <
            typename tag<Geometry>::type,
            Geometry,
            ggl::dimension<Geometry>::type::value
        >::apply(geometry, c1, c2, c3, c4);
}


/*!
    \brief assign a range of points to a linestring, ring or polygon
    \note The point-type of the range might be different from the point-type of the geometry
    \ingroup access
 */
template <typename Geometry, typename R>
inline void assign(Geometry& geometry, R const& range)
{
    concept::check<Geometry>();

    clear(geometry);
    ggl::append(geometry, range, -1, 0);
}


/*!
    \brief assign to a box inverse infinite
    \details The assign_inverse function initialize a 2D or 3D box with large coordinates, the
    min corner is very large, the max corner is very small. This is a convenient starting point to
    collect the minimum bounding box of a geometry.
    \ingroup access
 */
template <typename Geometry>
inline void assign_inverse(Geometry& geometry)
{
    concept::check<Geometry>();

    dispatch::assign_inverse
        <
            typename tag<Geometry>::type,
            Geometry
        >::apply(geometry);
}

/*!
    \brief assign zero values to a box, point
    \ingroup access
    \details The assign_zero function initializes a 2D or 3D point or box with coordinates of zero
    \tparam Geometry the geometry type
 */
template <typename Geometry>
inline void assign_zero(Geometry& geometry)
{
    concept::check<Geometry>();

    dispatch::assign_zero
        <
            typename tag<Geometry>::type,
            Geometry
        >::apply(geometry);
}


/*!
    \brief Assign the 4 points of a 2D box
    \ingroup assign
    \note The order can be crucial. Most logical is LOWER, UPPER and sub-order LEFT, RIGHT
*/
template <typename Box, typename Point>
inline void assign_box_corners(Box const& box,
        Point& lower_left, Point& lower_right,
        Point& upper_left, Point& upper_right)
{
    concept::check<const Box>();
    concept::check<Point>();

    detail::assign::assign_box_2d_corner<min_corner, min_corner>(box, lower_left);
    detail::assign::assign_box_2d_corner<max_corner, min_corner>(box, lower_right);
    detail::assign::assign_box_2d_corner<min_corner, max_corner>(box, upper_left);
    detail::assign::assign_box_2d_corner<max_corner, max_corner>(box, upper_right);
}



/*!
    \brief Assign a box with the value of a point
    \ingroup assign
    \tparam Corner indicates which box-corner, min_corner (0) or max_corner (1)
*/
template <std::size_t Corner, typename Box, typename Point>
inline void assign_point_to_box(Point const& point, Box& box)
{
    concept::check<const Point>();
    concept::check<Box>();

    detail::assign::assign_point_to_box
        <
            Box, Point, Corner, 0, dimension<Box>::type::value
        >::apply(point, box);
}



} // namespace ggl

#endif // GGL_ALGORITHMS_ASSIGN_HPP
