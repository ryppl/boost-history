// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_INTERSECTION_HPP
#define GGL_ALGORITHMS_INTERSECTION_HPP


#include <boost/mpl/if.hpp>

#include <ggl/core/reverse_dispatch.hpp>
#include <ggl/geometries/concepts/check.hpp>
#include <ggl/algorithms/overlay/clip_linestring.hpp>
#include <ggl/algorithms/overlay/assemble.hpp>



/*!
\defgroup intersection intersection operations
    (clipping, polygon-polygon intersection)
\details The intersection of two geometries A and B is the geometry containing
    all points of A also belonging to B, but no other elements. The so-called
    clip is an intersection of a geometry with a box.
\par Source description:
- OGC: Returns a geometric object that represents the Point set intersection of
    this geometric object with another Geometry.
\see http://en.wikipedia.org/wiki/Intersection_(set_theory)
\note Any intersection can result in no geometry at all

\par Performance
- 2776 counties of US are intersected with a 100-points ellipse in 1.1 seconds
(http://trac.osgeo.org/ggl/wiki/Performance#Interesection)
- 2776 counties of US are clipped in 0.2 seconds
(http://trac.osgeo.org/ggl/wiki/Performance#Clip)


\par Geometries:
- \b polygon + \b box (clip) -> \b polygon(s)
\image html svg_intersection_polygon_box.png
\image html svg_intersection_countries.png
- \b ring + \b box (clip) -> \b polygon(s)
\image html svg_intersection_ring_box.png
- \b ring + \b ring -> \b polygon(s)
\image html svg_intersection_ring_ring.png
- \b polygon + \b ring -> \b polygon(s)
\image html svg_intersection_polygon_ring.png
- combinations above -> \b ring(s).
    <i>If the output is an ouput iterator of rings, holes are omitted</i>
- \b linestring + \b box (clip)
\image html svg_intersection_roads.png

\par Example:
Example showing clipping of linestring with box
\dontinclude doxygen_examples.cpp
\skip example_clip_linestring1
\line {
\until }
\par Example:
Example showing clipping of vector, outputting vectors, with box
\dontinclude doxygen_examples.cpp
\skip example_clip_linestring2
\line {
\until }
\par Example:
Example showing clipping of polygon with box
\dontinclude doxygen_examples.cpp
\skip example_intersection_polygon1
\line {
\until }
*/

namespace ggl
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace overlay {



// Specializations for "take_one" for intersection
// "one" should be the inner one

// for ring and box
template
<
    typename Tag1, typename Geometry1,
    typename Tag2, typename Geometry2,
    typename GeometryOut
>
struct take_if_1_is_in_2<Tag1, Geometry1, Tag2, Geometry2, GeometryOut, -1>
{
    static inline void apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2,
                GeometryOut& out
                )
    {
        ggl::convert(geometry1, out);
    }
};


template
<
    typename Geometry1,
    typename Tag2, typename Geometry2,
    typename GeometryOut
>
struct take_if_1_is_in_2<polygon_tag, Geometry1, Tag2, Geometry2, GeometryOut, -1>
{
    static inline void apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2,
                GeometryOut& out
                )
    {
        ggl::convert(exterior_ring(geometry1), out);
    }
};


template
<
    typename Geometry, typename GeometryOut, typename Container
>
struct add_holes<polygon_tag, Geometry, GeometryOut, Container, -1>
{

    static inline void apply(Geometry const& geometry,
                GeometryOut& out,
                Container const& holes
                )
    {
        std::vector<sortable> v;
        sort_interior_rings(holes, v, -1);

        // For an intersection, if a ring is containing an inner ring,
        // take the outer

        std::size_t const n = boost::size(v);

        for (std::size_t i = 0; i < n; i++)
        {
            // So, only if no inners:
            if (v[i].index_of_parent == -1)
            {
                typename ggl::point_type<GeometryOut>::type point;
                ggl::point_on_border(holes[v[i].index], point);
                if (ggl::within(point, geometry))
                {
                    typename ring_type<GeometryOut>::type hole;
                    ggl::convert(holes[v[i].index], hole);
                    ggl::interior_rings(out).push_back(hole);
                }
            }
        }
    }
};


}} // namespace detail::overlay
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag1, typename Tag2, typename Tag3,
    typename G1, typename G2,
    typename OutputIterator,
    typename GeometryOut
>
struct intersection_inserter {};


template
<
    typename Segment1, typename Segment2,
    typename OutputIterator, typename GeometryOut
>
struct intersection_inserter
    <
        segment_tag, segment_tag, point_tag,
        Segment1, Segment2,
        OutputIterator, GeometryOut
    >
{
    static inline OutputIterator apply(Segment1 const& segment1,
            Segment2 const& segment2, OutputIterator out)
    {
        typedef typename point_type<GeometryOut>::type point_type;

        // Get the intersection point (or two points)
        segment_intersection_points<point_type> is
            = strategy::intersection::relate_cartesian_segments
            <
                policies::relate::segments_intersection_points
                    <
                        Segment1,
                        Segment2,
                        segment_intersection_points<point_type>
                    >
            >::relate(segment1, segment2);
        for (int i = 0; i < is.count; i++)
        {
            GeometryOut p;
            ggl::copy_coordinates(is.intersections[i], p);
            *out = p;
            out++;
        }
        return out;
    }
};


template
<
    typename Linestring, typename Box,
    typename OutputIterator, typename GeometryOut
>
struct intersection_inserter
    <
        linestring_tag, box_tag, linestring_tag,
        Linestring, Box,
        OutputIterator, GeometryOut
    >
{
    static inline OutputIterator apply(Linestring const& linestring,
            Box const& box, OutputIterator out)
    {
        typedef typename point_type<GeometryOut>::type point_type;
        strategy::intersection::liang_barsky<Box, point_type> strategy;
        return detail::intersection::clip_linestring_with_box<GeometryOut>(box, linestring, out, strategy);
    }
};


template
<
    typename Polygon1, typename Polygon2,
    typename OutputIterator, typename GeometryOut
>
struct intersection_inserter
    <
        polygon_tag, polygon_tag, polygon_tag,
        Polygon1, Polygon2,
        OutputIterator, GeometryOut
    >
    : detail::overlay::overlay_and_assemble
        <Polygon1, Polygon2, OutputIterator, GeometryOut, -1>
{};



template
<
    typename Polygon, typename Box,
    typename OutputIterator, typename GeometryOut
>
struct intersection_inserter
<
    polygon_tag, box_tag, polygon_tag,
    Polygon, Box,
    OutputIterator, GeometryOut
>
    : detail::overlay::overlay_and_assemble
        <Polygon, Box, OutputIterator, GeometryOut, -1>
{};



template
<
    typename GeometryTag1, typename GeometryTag2, typename GeometryTag3,
    typename Geometry1, typename Geometry2,
    typename OutputIterator, typename GeometryOut
>
struct intersection_inserter_reversed
{
    static inline OutputIterator apply(Geometry1 const& g1,
                Geometry2 const& g2, OutputIterator out)
    {
        return intersection_inserter
            <
                GeometryTag2, GeometryTag1, GeometryTag3,
                Geometry2, Geometry1,
                OutputIterator, GeometryOut
            >::apply(g2, g1, out);
    }
};



} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH

/*!
    \brief Intersects two geometries which each other
    \ingroup intersection
    \details The two input geometries are intersected and the resulting linestring(s),
    ring(s) or polygon(s) are sent to the specified output operator.
    \tparam GeometryOut output geometry type, must be specified
    \tparam Geometry1 first geometry type
    \tparam Geometry2 second geometry type
    \tparam OutputIterator output iterator
    \param geometry1 first geometry
    \param geometry2 second geometry
    \param out the output iterator, outputting linestrings or polygons
    \return the output iterator
*/
template
<
    typename GeometryOut,
    typename Geometry1,
    typename Geometry2,
    typename OutputIterator
>
inline OutputIterator intersection(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            OutputIterator out)
{
    concept::check<const Geometry1>();
    concept::check<const Geometry2>();

    return boost::mpl::if_c
        <
            reverse_dispatch<Geometry1, Geometry2>::type::value,
            dispatch::intersection_inserter_reversed
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<GeometryOut>::type,
                Geometry1,
                Geometry2,
                OutputIterator, GeometryOut
            >,
            dispatch::intersection_inserter
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<GeometryOut>::type,
                Geometry1,
                Geometry2,
                OutputIterator, GeometryOut
            >
        >::type::apply(geometry1, geometry2, out);
}


} // ggl


#endif //GGL_ALGORITHMS_INTERSECTION_HPP
