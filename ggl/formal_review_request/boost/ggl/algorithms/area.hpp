// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_AREA_HPP
#define GGL_ALGORITHMS_AREA_HPP

#include <boost/mpl/if.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>


#include <ggl/core/point_order.hpp>
#include <ggl/core/exterior_ring.hpp>
#include <ggl/core/interior_rings.hpp>
#include <ggl/core/ring_type.hpp>

#include <ggl/geometries/concepts/check.hpp>

#include <ggl/algorithms/detail/calculate_null.hpp>
#include <ggl/algorithms/detail/calculate_sum.hpp>

#include <ggl/strategies/strategies.hpp>
#include <ggl/strategies/area_result.hpp>

#include <ggl/strategies/concepts/area_concept.hpp>

#include <ggl/util/math.hpp>

/*!
\defgroup area area calculation

\par Performance
2776 * 1000 area calculations are done in 0.11 seconds
(other libraries: 0.125 seconds, 0.125 seconds, 0.5 seconds)

\par Coordinate systems and strategies
Area calculation can be done in Cartesian and in spherical/geographic
coordinate systems.

\par Geometries
The area algorithm calculates the surface area of all geometries
having a surface: box, polygon, multi_polygon. The units are the square of
the units used for the points defining the surface. If the polygon is defined
in meters, the area is in square meters.

\par Example:
Example showing area calculation of polygons built
\dontinclude doxygen_examples.cpp
\skip example_area_polygon()
\line {
\until }

*/
namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace area {

template<typename Box, typename Strategy>
struct box_area
{
    typedef typename coordinate_type<Box>::type return_type;

    static inline return_type apply(Box const& box, Strategy const&)
    {
        // Currently only works for 2D Cartesian boxes
        // Todo: use extreme-strategy
        assert_dimension<Box, 2>();

        return_type const dx = get<max_corner, 0>(box) 
                - get<min_corner, 0>(box);
        return_type const dy = get<max_corner, 1>(box) 
                - get<min_corner, 1>(box);

        return dx * dy;
    }
};




/*!
    \brief Calculate area of a ring, specialized per order
 */
template
<
    typename Ring,
    order_selector Order,
    // closing_selector Closed -- for now assuming CLOSED, p(0) == p(n-1)
    typename Strategy
>
struct ring_area
{};


template<typename Ring, typename Strategy>
struct ring_area<Ring, clockwise, Strategy>
{
    BOOST_CONCEPT_ASSERT( (ggl::concept::AreaStrategy<Strategy>) );

    typedef typename Strategy::return_type type;

    static inline type apply(Ring const& ring, Strategy const& strategy)
    {
        assert_dimension<Ring, 2>();

        // A closed linear_ring has at least four points,
        // if not, there is no (zero) area
        if (boost::size(ring) < 4)
        {
            return type();
        }

        typedef typename boost::range_const_iterator<Ring>::type iterator_type;

        typename Strategy::state_type state = strategy.init();

        iterator_type it = boost::begin(ring);
        for (iterator_type previous = it++;
            it != boost::end(ring);
            previous = it++)
        {
            Strategy::apply(*previous, *it, state);
        }
        return Strategy::result(state);

    }
};

template<typename Ring, typename Strategy>
struct ring_area<Ring, counterclockwise, Strategy>
{
    typedef typename Strategy::return_type type;
    static inline type apply(Ring const& ring, Strategy const& strategy)
    {
        // Counter clockwise rings negate the area result
        return -ring_area<Ring, clockwise, Strategy>::apply(ring, strategy);
    }
};


}} // namespace detail::area

#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch {

template
<
    typename Tag,
    typename Geometry,
    order_selector Order,
    typename Strategy
>
struct area
    : detail::calculate_null
        <
            typename Strategy::return_type,
            Geometry,
            Strategy
        > {};


template <typename Geometry, order_selector Order, typename Strategy>
struct area<box_tag, Geometry, Order, Strategy>
    : detail::area::box_area<Geometry, Strategy>
{};




// Area of ring currently returns area of closed rings but it might be argued
// that it is 0.0, because a ring is just a line.
template <typename Geometry, order_selector Order, typename Strategy>
struct area<ring_tag, Geometry, Order, Strategy>
    : detail::area::ring_area<Geometry, Order, Strategy>
{};

template <typename Polygon, order_selector Order, typename Strategy>
struct area<polygon_tag, Polygon, Order, Strategy>
    : detail::calculate_polygon_sum
        <
            typename Strategy::return_type,
            Polygon,
            Strategy,
            detail::area::ring_area
                <
                    typename ring_type<Polygon>::type,
                    Order,
                    Strategy
                >
        >
    //: detail::area::polygon_area<Geometry, Order, Strategy>
{};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH



/*!
    \brief Calculate area of a geometry
    \ingroup area
    \details The function area returns the area of a polygon, ring, box
    using the default area-calculation strategy. Strategies are
    provided for cartesian and spherical coordinate systems
    The geometries should correct, polygons should be closed
    and according to the specified orientation (clockwise/counter clockwise)
    \param geometry a geometry
    \return the area
 */
template <typename Geometry>
inline typename area_result<Geometry>::type area(Geometry const& geometry)
{
    concept::check<const Geometry>();

    typedef typename area_result<Geometry>::strategy_type strategy_type;

    return dispatch::area
        <
            typename tag<Geometry>::type,
            Geometry,
            ggl::point_order<Geometry>::value,
            strategy_type
        >::apply(geometry, strategy_type());
}

/*!
    \brief Calculate area of a geometry using a strategy
    \ingroup area
    \details This version of area calculation takes a strategy
    \param geometry a geometry
    \param strategy the strategy to calculate area.
        Especially for spherical areas there are some approaches.
    \return the area
 */
template <typename Geometry, typename Strategy>
inline typename Strategy::return_type area(
        Geometry const& geometry, Strategy const& strategy)
{
    concept::check<const Geometry>();

    return dispatch::area
        <
            typename tag<Geometry>::type,
            Geometry,
            ggl::point_order<Geometry>::value,
            Strategy
        >::apply(geometry, strategy);
}

} // namespace ggl

#endif // GGL_ALGORITHMS_AREA_HPP
