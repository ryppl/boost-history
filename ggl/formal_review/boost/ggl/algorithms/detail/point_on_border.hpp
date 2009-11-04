// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_DETAIL_POINT_ON_BORDER_HPP
#define GGL_ALGORITHMS_DETAIL_POINT_ON_BORDER_HPP


#include <boost/range/functions.hpp>

#include <ggl/core/point_type.hpp>
#include <ggl/core/ring_type.hpp>

#include <ggl/geometries/concepts/check.hpp>

#include <ggl/algorithms/assign.hpp>


namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace point_on_border {

template
<
    typename Range,
    typename Point
>
struct point_on_range
{
    static inline bool apply(Range const& range, Point& point)
    {
        if (boost::size(range) > 0)
        {
            point = *boost::begin(range);
            return true;
        }
        return false;
    }
};


template
<
    typename Polygon,
    typename Point
>
struct point_on_polygon
{
    static inline bool apply(Polygon const& polygon, Point& point)
    {
        return point_on_range
            <
                typename ring_type<Polygon>::type,
                Point
            >::apply(exterior_ring(polygon), point);
    }
};


template
<
    typename Box,
    typename Point
>
struct point_on_box
{
    static inline bool apply(Box const& box, Point& point)
    {
        detail::assign::assign_box_2d_corner<min_corner, min_corner>(box, point);
        return true;
    }
};


}} // namespace detail::point_on_border
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename GeometryTag,
    typename Geometry,
    typename Point

>
struct point_on_border
{
};


template<typename Point>
struct point_on_border
    <
        point_tag, Point, Point
    >
{
    static inline bool apply(Point const& source, Point& destination)
    {
        destination = source;
        return true;
    }
};


template<typename Linestring, typename Point>
struct point_on_border<linestring_tag, Linestring, Point>
    : detail::point_on_border::point_on_range<Linestring, Point>
{};


template<typename Ring, typename Point>
struct point_on_border<ring_tag, Ring, Point>
    : detail::point_on_border::point_on_range<Ring, Point>
{};


template<typename Polygon, typename Point>
struct point_on_border<polygon_tag, Polygon, Point>
    : detail::point_on_border::point_on_polygon<Polygon, Point>
{};


template<typename Box, typename Point>
struct point_on_border<box_tag, Box, Point>
    : detail::point_on_border::point_on_box<Box, Point>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
    \brief Take point on a border
    \ingroup utility 
    \tparam Geometry geometry type
    \param geometry geometry to take point from
    \param point 
    \return TRUE if successful, else false. 
        It is only false if polygon/line have no points
    \note for a polygon, it is always a point on the exterior ring
 */
template <typename Geometry>
inline bool point_on_border(Geometry const& geometry,
    typename point_type<Geometry>::type& point)
{
    typedef typename point_type<Geometry>::type point_type;

    concept::check<const Geometry>();
    concept::check<point_type>();

    return dispatch::point_on_border
            <
                typename tag<Geometry>::type,
                Geometry,
                point_type
            >::apply(geometry, point);
}



} // namespace ggl

#endif // GGL_ALGORITHMS_DETAIL_POINT_ON_BORDER_HPP
