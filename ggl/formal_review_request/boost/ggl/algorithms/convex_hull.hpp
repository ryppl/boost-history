// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_CONVEX_HULL_HPP
#define GGL_ALGORITHMS_CONVEX_HULL_HPP


#include <boost/type_traits/remove_const.hpp>

#include <ggl/core/cs.hpp>
#include <ggl/core/is_multi.hpp>
#include <ggl/core/point_order.hpp>
#include <ggl/core/exterior_ring.hpp>

#include <ggl/geometries/concepts/check.hpp>


#include <ggl/strategies/strategies.hpp>
#include <ggl/util/as_range.hpp>


/*!
\defgroup convex_hull convex hull calculation
\par Source descriptions:
- OGC description: Returns a geometric object that represents the convex hull of
    this geometric object. Convex hulls, being dependent on straight lines, can
    be accurately represented in linear interpolations
    for any geometry restricted to linear interpolations.
\see http://en.wikipedia.org/wiki/Convex_hull

\par Performance
2776 counties of US are "hulled" in 0.9 seconds (other libraries: 0.9, 4.1, 3.3, 1.4 seconds)

\note The convex hull is always a ring, holes are not possible. Therefore it is
    can also be used in combination with an output iterator.
\par Geometries:
In the images below the convex hull is painted in red.
- POINT: will not compile
- POLYGON: will deliver a polygon without holes
    \image html convexhull_polygon_polygon.png
*/
namespace ggl {

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace convex_hull {

template
<
    typename Geometry,
    order_selector Order
>
struct hull_inserter
{
    // Member template function, to avoid inconvenient declaration
    // of output-iterator-type
    template <typename OutputIterator>
    static inline OutputIterator apply(Geometry const& geometry,
            OutputIterator out)
    {
        typedef typename point_type<Geometry>::type point_type;

        typedef typename strategy_convex_hull
            <
                typename cs_tag<point_type>::type,
                point_type
            >::type strategy_type;

        strategy_type s(as_range
                <
                    typename as_range_type<Geometry>::type
                >(geometry));
        s.get(out, Order == clockwise);
        return out;
    }
};

template
<
    typename Geometry,
    typename OutputGeometry
>
struct hull_to_geometry
{
    static inline void apply(Geometry const& geometry, OutputGeometry& out)
    {
        hull_inserter
            <
                Geometry,
                ggl::point_order<OutputGeometry>::value
            >::apply(geometry,
                std::back_inserter(
                    ggl::as_range
                        <
                            typename ggl::as_range_type<OutputGeometry>::type
                        >(out)));
    }
};




}} // namespace detail::convex_hull
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag1,
    bool IsMulti,
    typename Polygon, typename Output
>
struct convex_hull
{};


template <typename Polygon, typename Output>
struct convex_hull
<
    polygon_tag, false,
    Polygon, Output
>
    : detail::convex_hull::hull_to_geometry<Polygon, Output>
{};

template <typename Polygon, typename Output>
struct convex_hull
<
    ring_tag, false,
    Polygon, Output
>
    : detail::convex_hull::hull_to_geometry<Polygon, Output>
{};

template <typename Polygon, typename Output>
struct convex_hull
<
    linestring_tag, false,
    Polygon, Output
>
    : detail::convex_hull::hull_to_geometry<Polygon, Output>
{};



template
<
    typename GeometryTag,
    order_selector Order,
    bool IsMulti,
    typename GeometryIn
 >
struct convex_hull_inserter {};

template <typename Linestring, order_selector Order>
struct convex_hull_inserter
<
    linestring_tag,
    Order, false,
    Linestring
>
    : detail::convex_hull::hull_inserter<Linestring, Order>
{};


template <typename Ring, order_selector Order>
struct convex_hull_inserter
<
    ring_tag,
    Order, false,
    Ring
>
    : detail::convex_hull::hull_inserter<Ring, Order>
{};


template <typename Polygon, order_selector Order>
struct convex_hull_inserter
<
    polygon_tag,
    Order, false,
    Polygon
>
    : detail::convex_hull::hull_inserter<Polygon, Order>
{};



} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH



/*!
    \brief Calculate the convex hull of a geometry
    \ingroup convex_hull
    \tparam Geometry1 the input geometry type
    \tparam Geometry2: the output geometry type
    \param geometry the geometry to calculate convex hull from
    \param out a geometry receiving points of the convex hull
    \note the output may be:
    - a polygon
    - a ring

 */
template<typename Geometry1, typename Geometry2>
inline void convex_hull(Geometry1 const& geometry,
            Geometry2& out)
{
    concept::check<const Geometry1>();
    concept::check<Geometry2>();

    dispatch::convex_hull
        <
            typename tag<Geometry1>::type,
            is_multi<Geometry1>::type::value,
            Geometry1,
            Geometry2
        >::apply(geometry, out);
}


/*!
    \brief Calculate the convex hull of a geometry, output-iterator version
    \ingroup convex_hull
    \tparam Geometry the input geometry type
    \tparam OutputIterator: an output-iterator
    \param geometry the geometry to calculate convex hull from
    \param out an output iterator outputing points of the convex hull
    \note This overloaded version outputs to an output iterator.
    In this case, nothing is known about its point-type or
        about its clockwise order. Therefore, the input point-type
        and order are copied

 */
template<typename Geometry, typename OutputIterator>
inline OutputIterator convex_hull_inserter(Geometry const& geometry,
            OutputIterator out)
{
    // Concept: output point type = point type of input geometry
    concept::check<const Geometry>();
    concept::check<typename point_type<Geometry>::type>();

    return dispatch::convex_hull_inserter
        <
            typename tag<Geometry>::type,
            ggl::point_order<Geometry>::value,
            is_multi<Geometry>::type::value,
            Geometry
        >::apply(geometry, out);
}


} // namespace ggl

#endif // GGL_ALGORITHMS_CONVEX_HULL_HPP
