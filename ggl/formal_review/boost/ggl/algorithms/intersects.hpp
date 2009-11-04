// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_INTERSECTS_HPP
#define GGL_ALGORITHMS_INTERSECTS_HPP


/*!
\defgroup intersects intersects: detect if a geometry self-intersects or if two geometries intersect
\par Source descriptions:
- OGC description: Returns 1 (TRUE) if  this geometric object spatially
    intersects anotherGeometry.
- OGC: a.Intersects(b) <=> ! a.Disjoint(b)
\note There are two overloaded versions:
- with one geometry, detecting self-intersections
- with two geometries, deferring to disjoint, returning !disjoint

\par Geometries:
- \b ring
- \b polygon
- for two geometries: same is disjoint

\note if one geometry is completely within another geometry, it "intersects"

*/


#include <ggl/geometries/concepts/check.hpp>


#include <ggl/algorithms/overlay/get_intersection_points.hpp>
#include <ggl/algorithms/overlay/self_intersection_points.hpp>
#include <ggl/algorithms/overlay/adapt_turns.hpp>
#include <ggl/algorithms/overlay/enrich_intersection_points.hpp>
#include <ggl/algorithms/overlay/traverse.hpp>

#include <ggl/algorithms/disjoint.hpp>



namespace ggl
{

/*!
    \brief Determine if there is at least one intersection
        (crossing or self-tangency)
    \note This function can be called for one geometry (self-intersection) and
        also for two geometries (intersection)
    \ingroup intersects
    \tparam Geometry geometry type
    \param geometry geometry
    \return true if there are intersections, else false
 */
template <typename Geometry>
inline bool intersects(Geometry const& geometry)
{
    concept::check<const Geometry>();

    typedef typename boost::remove_const<Geometry>::type ncg_type;

    typedef std::vector<ggl::detail::intersection::intersection_point
        <typename ggl::point_type<Geometry>::type> > ip_vector;

    ip_vector ips;

    dispatch::self_intersection_points
            <
                typename tag<ncg_type>::type,
                is_multi<ncg_type>::type::value,
                ncg_type,
                ip_vector
            >::apply(geometry, true, ips);
    return ips.size() > 0;
}


/*!
    \brief Determine if there is at least one intersection
    \ingroup intersects
    \tparam Geometry1 first geometry type
    \tparam Geometry2 second geometry type
    \param geometry1 first geometry
    \param geometry2 second geometry
    \return true if there are intersection(s), else false
 */
template <typename Geometry1, typename Geometry2>
inline bool intersects(Geometry1 const& geometry1, Geometry2 const& geometry2)
{
    concept::check<const Geometry1>();
    concept::check<const Geometry2>();

    return ! ggl::disjoint(geometry1, geometry2);
}



} // ggl

#endif //GGL_ALGORITHMS_INTERSECTS_HPP
