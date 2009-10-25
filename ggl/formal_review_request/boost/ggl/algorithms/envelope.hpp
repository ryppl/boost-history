// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_ENVELOPE_HPP
#define GGL_ALGORITHMS_ENVELOPE_HPP

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <ggl/algorithms/combine.hpp>
#include <ggl/algorithms/convert.hpp>
#include <ggl/core/cs.hpp>
#include <ggl/core/exterior_ring.hpp>

#include <ggl/geometries/concepts/check.hpp>


#include <ggl/strategies/strategies.hpp>

/*!
\defgroup envelope envelope calculation
\par Source descriptions:
- OGC: Envelope (): Geometry - The minimum bounding rectangle (MBR) for this
    Geometry,
returned as a Geometry. The polygon is defined by the corner points of the
    bounding box
    [(MINX, MINY), (MAXX, MINY), (MAXX, MAXY), (MINX, MAXY), (MINX, MINY)].

\note Implemented in the Generic Geometry Library: The minimum bounding box,
    always as a box, having min <= max

The envelope algorithm calculates the bounding box, or envelope, of a geometry.
There are two versions:
- envelope, taking a reference to a box as second parameter
- make_envelope, returning a newly constructed box (type as a template parameter in the function call)
- either of them has an optional strategy

\par Geometries:
- POINT: a box with zero area, the maximum and the minimum point of the box are
set to the point itself.
- LINESTRING, RING or RANGE is the smallest box that contains all points of the
    specified point sequence.
If the linestring is empty, the envelope is the inverse infinite box, that is,
    the minimum point is very large (max infinite) and the maximum point is
    very small (min infinite).
- POLYGON, the envelope of the outer ring
\image html envelope_polygon.png

\par Example:
Example showing envelope calculation
\dontinclude doxygen_examples.cpp
\skip example_envelope_linestring
\line {
\until }
*/

namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace envelope {



/// Calculate envelope of an 2D or 3D point
template<typename Point, typename Box, typename Strategy>
struct envelope_point
{
    static inline void apply(Point const& p, Box& mbr, Strategy const&)
    {
        // Envelope of a point is an empty box, a box with zero volume, located
        // at the point.
        // We just use the convert algorithm here
        ggl::convert(p, mbr);
    }
};


/// Calculate envelope of an 2D or 3D segment
template<typename Segment, typename Box, typename Strategy>
struct envelope_segment
{
    static inline void apply(Segment const& s, Box& mbr, Strategy const&)
    {
        // TODO: remove s.first/s.second, use strategies
        ggl::assign_inverse(mbr);
        ggl::combine(mbr, s.first);
        ggl::combine(mbr, s.second);
    }
};


/// Version with state iterating through range (also used in multi*)
template<typename Range, typename Strategy>
inline void envelope_range_state(Range const& range,
        Strategy const& strategy, typename Strategy::state_type& state)
{
    typedef typename boost::range_const_iterator<Range>::type iterator_type;

    for (iterator_type it = boost::begin(range);
        it != boost::end(range);
        ++it)
    {
        strategy(*it, state);
    }
}



/// Generic range dispatching struct
template <typename Range, typename Box, typename Strategy>
struct envelope_range
{
    /// Calculate envelope of range using a strategy
    static inline void apply(Range const& range, Box& mbr,
            Strategy const& strategy)
    {
        typename Strategy::state_type state(mbr);
        envelope_range_state(range, strategy, state);
    }
};

}} // namespace detail::envelope
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag1, typename Tag2,
    typename Geometry, typename Box,
    typename Strategy
>
struct envelope {};


template <typename Point, typename Box, typename Strategy>
struct envelope<point_tag, box_tag, Point, Box, Strategy>
    : detail::envelope::envelope_point<Point, Box, Strategy>
{};


template <typename Box, typename Strategy>
struct envelope<box_tag, box_tag, Box, Box, Strategy>
{
    /*!
        \brief Calculate envelope of a box
        \details The envelope of a box is itself, provided
        for consistency, on itself it is not useful.
     */
    static inline void apply(Box const& box, Box& mbr, Strategy const&)
    {
        mbr = box;
    }
};


template <typename Segment, typename Box, typename Strategy>
struct envelope<segment_tag, box_tag, Segment, Box, Strategy>
    : detail::envelope::envelope_segment<Segment, Box, Strategy>
{};



template <typename Linestring, typename Box, typename Strategy>
struct envelope<linestring_tag, box_tag, Linestring, Box, Strategy>
    : detail::envelope::envelope_range<Linestring, Box, Strategy>
{};


template <typename Ring, typename Box, typename Strategy>
struct envelope<ring_tag, box_tag, Ring, Box, Strategy>
    : detail::envelope::envelope_range<Ring, Box, Strategy>
{};


template <typename Polygon, typename Box, typename Strategy>
struct envelope<polygon_tag, box_tag, Polygon, Box, Strategy>
{
    static inline void apply(Polygon const& poly, Box& mbr,
            Strategy const& strategy)
    {
        // For polygon, inspecting outer ring is sufficient

        detail::envelope::envelope_range
            <
                typename ring_type<Polygon>::type,
                Box,
                Strategy
            >::apply(exterior_ring(poly), mbr, strategy);
    }

};


} // namespace dispatch
#endif


/*!
\brief Calculate envelope of a geometry, using a specified strategy
\ingroup envelope
\param geometry the geometry
\param mbr the box receiving the envelope
\param strategy strategy to be used
*/
template<typename Geometry, typename Box, typename Strategy>
inline void envelope(Geometry const& geometry,
        Box& mbr, Strategy const& strategy)
{
    concept::check<const Geometry>();
    concept::check<Box>();

    dispatch::envelope
        <
            typename tag<Geometry>::type, typename tag<Box>::type,
            Geometry, Box, Strategy
        >::apply(geometry, mbr, strategy);
}




/*!
\brief Calculate envelope of a geometry
\ingroup envelope
\param geometry the geometry
\param mbr the box receiving the envelope
\par Example:
Example showing envelope calculation, using point_ll latlong points
\dontinclude doxygen_examples.cpp
\skip example_envelope_polygon
\line {
\until }
*/
template<typename Geometry, typename Box>
inline void envelope(Geometry const& geometry, Box& mbr)
{
    concept::check<const Geometry>();
    concept::check<Box>();

    typename strategy_envelope
        <
            typename cs_tag<typename point_type<Geometry>::type>::type,
            typename cs_tag<typename point_type<Box>::type>::type,
            typename point_type<Geometry>::type,
            Box
        >::type strategy;

    envelope(geometry, mbr, strategy);
}


/*!
\brief Calculate and return envelope of a geometry
\ingroup envelope
\param geometry the geometry
\param strategy the strategy to be used
*/
template<typename Box, typename Geometry, typename Strategy>
inline Box make_envelope(Geometry const& geometry, Strategy const& strategy)
{
    concept::check<const Geometry>();
    concept::check<Box>();

    Box box;
    dispatch::envelope
        <
            typename tag<Geometry>::type, typename tag<Box>::type,
            Geometry, Box, Strategy
        >::apply(geometry, box, strategy);

    return box;
}


/*!
\brief Calculate and return envelope of a geometry
\ingroup envelope
\param geometry the geometry
*/
template<typename Box, typename Geometry>
inline Box make_envelope(Geometry const& geometry)
{
    concept::check<const Geometry>();
    concept::check<Box>();

    typename strategy_envelope
        <
            typename cs_tag<typename point_type<Geometry>::type>::type,
            typename cs_tag<typename point_type<Box>::type>::type,
            typename point_type<Geometry>::type,
            Box
        >::type strategy;
    return make_envelope<Box>(geometry, strategy);
}


} // namespace ggl

#endif // GGL_ALGORITHMS_ENVELOPE_HPP
