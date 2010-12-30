// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_UTIL_FOR_EACH_RANGE_HPP
#define BOOST_GEOMETRY_UTIL_FOR_EACH_RANGE_HPP


#include <boost/concept/requires.hpp>


#include <boost/geometry/core/tag.hpp>
#include <boost/geometry/core/is_multi.hpp>

#include <boost/geometry/util/add_const_if_c.hpp>


namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace for_each
{


template <typename Range, typename Actor, bool IsConst>
struct fe_range_range
{
    static inline void apply(
                    typename add_const_if_c<IsConst, Range>::type& range,
                    Actor& actor)
    {
        actor.apply(range);
    }
};


template <typename Polygon, typename Actor, bool IsConst>
struct fe_range_polygon
{
    static inline void apply(
                    typename add_const_if_c<IsConst, Polygon>::type& polygon,
                    Actor& actor)
    {
        actor.apply(exterior_ring(polygon));

        // TODO: If some flag says true, also do the inner rings.
        // for convex hull, it's not necessary
    }
};


}} // namespace detail::for_each
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template
<
    typename Tag,
    bool IsMulti,
    typename Geometry,
    typename Actor,
    bool IsConst
>
struct for_each_range {};


template <typename Linestring, typename Actor, bool IsConst>
struct for_each_range<linestring_tag, false, Linestring, Actor, IsConst>
    : detail::for_each::fe_range_range<Linestring, Actor, IsConst>
{};


template <typename Ring, typename Actor, bool IsConst>
struct for_each_range<ring_tag, false, Ring, Actor, IsConst>
    : detail::for_each::fe_range_range<Ring, Actor, IsConst>
{};


template <typename Polygon, typename Actor, bool IsConst>
struct for_each_range<polygon_tag, false, Polygon, Actor, IsConst>
    : detail::for_each::fe_range_polygon<Polygon, Actor, IsConst>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


template <typename Geometry, typename Actor>
inline void for_each_range(Geometry const& geometry, Actor& actor)
{
    dispatch::for_each_range
        <
            typename tag<Geometry>::type,
            is_multi<Geometry>::type::value,
            Geometry,
            Actor,
            true
        >::apply(geometry, actor);
}


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_UTIL_FOR_EACH_RANGE_HPP
