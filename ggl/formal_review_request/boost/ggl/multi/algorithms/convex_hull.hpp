// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_ALGORITHMS_CONVEX_HULL_HPP
#define GGL_MULTI_ALGORITHMS_CONVEX_HULL_HPP


#include <ggl/util/as_range.hpp>

#include <ggl/multi/core/tags.hpp>
#include <ggl/multi/core/is_multi.hpp>
#include <ggl/multi/core/point_type.hpp>

#include <ggl/algorithms/convex_hull.hpp>


namespace ggl {


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace convex_hull {


template 
<
    typename MultiGeometry, 
    order_selector Order
>
struct multi_inserter
{
    template <typename OutputIterator>
    static inline OutputIterator apply(MultiGeometry const& multi,
            OutputIterator out)
    {
        using namespace boost;

        typedef typename point_type<MultiGeometry>::type point;
        typename strategy_convex_hull
            <
                typename cs_tag<point>::type,
                point
            >::type strategy;

        typedef typename range_const_iterator<MultiGeometry>::type iterator;
        typedef typename range_value<MultiGeometry>::type single;
        for(iterator it = begin(multi); it != end(multi); ++it)
        {
            strategy.add_range(as_range
                    <typename as_range_type<single>::type>(*it));
        }
        strategy.handle_input();

        strategy.get(out, Order == clockwise);
        return out;
    }
};



template
<
    typename Geometry,
    typename OutputGeometry
>
struct multi_hull_to_geometry
{
    static inline void apply(Geometry const& geometry, OutputGeometry& out)
    {
        multi_inserter
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

#endif



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


// Specialize for multi's
template
<
    typename MultiTag,
    order_selector Order,
    typename MultiGeometry
>
struct convex_hull_inserter<MultiTag, Order, true, MultiGeometry>
    : detail::convex_hull::multi_inserter<MultiGeometry, Order> 
{};


// Specialize for point
template 
<
    order_selector Order,
    typename MultiPoint
>
struct convex_hull_inserter<multi_point_tag, Order, true, MultiPoint>
    : detail::convex_hull::hull_inserter<MultiPoint, Order> 
{};


// Versions outputting to a ring or polygon
template <typename MultiTag, typename MultiGeometry, typename Output>
struct convex_hull
<
    MultiTag, true,
    MultiGeometry, Output
>
    : detail::convex_hull::multi_hull_to_geometry<MultiGeometry, Output>
{};


template <typename MultiGeometry, typename Output>
struct convex_hull
<
    multi_point_tag, true,
    MultiGeometry, Output
>
    : detail::convex_hull::hull_to_geometry<MultiGeometry, Output>
{};





} // namespace dispatch
#endif


} // namespace ggl


#endif // GGL_MULTI_ALGORITHMS_CONVEX_HULL_HPP
