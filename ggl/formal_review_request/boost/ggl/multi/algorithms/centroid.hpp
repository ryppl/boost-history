// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_ALGORITHMS_CENTROID_HPP
#define GGL_MULTI_ALGORITHMS_CENTROID_HPP

#include <ggl/algorithms/centroid.hpp>
#include <ggl/multi/core/point_type.hpp>
#include <ggl/multi/algorithms/detail/multi_sum.hpp>

namespace ggl {


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace centroid {

template
<
    typename Multi, 
    typename Point,
    typename Strategy 
>
struct centroid_multi_point
{
    static inline bool apply(Multi const& multi, Point& centroid,
            Strategy const& strategy, typename Strategy::state_type& state)
    {
        assign_zero(centroid);
        int n = 0;

        for (typename boost::range_const_iterator<Multi>::type 
                it = boost::begin(multi); 
            it != boost::end(multi); 
            ++it)
        {
            add_point(centroid, *it);
            n++;
        }
        divide_value(centroid, n);

        return false;
    }
};


template
<
    typename Multi, 
    typename Point,
    typename Strategy, 
    typename Policy
>
struct centroid_multi
{
    static inline bool apply(Multi const& multi, Point& centroid,
            Strategy const& strategy, typename Strategy::state_type& state)
    {
        for (typename boost::range_const_iterator<Multi>::type 
                it = boost::begin(multi); 
            it != boost::end(multi); 
            ++it)
        {
            Policy::apply(*it, centroid, strategy, state);
        }
        return true;
    }
};



}} // namespace detail::centroid
#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename MultiPolygon, typename Point, typename Strategy>
struct centroid<multi_polygon_tag, 2, MultiPolygon, Point, Strategy>
    : detail::centroid::centroid_multi
        <
            MultiPolygon, 
            Point,
            Strategy,
            detail::centroid::centroid_polygon
                <
                    typename boost::range_value<MultiPolygon>::type,
                    Point,
                    Strategy
                >
        >
{};


template
<
    std::size_t Dimensions,
    typename MultiPoint, 
    typename Point, 
    typename Strategy
>
struct centroid<multi_point_tag, Dimensions, MultiPoint, Point, Strategy>
    : detail::centroid::centroid_multi_point
        <
            MultiPoint, 
            Point,
            Strategy
        >
{};


} // namespace dispatch
#endif


} // namespace ggl


#endif // GGL_MULTI_ALGORITHMS_CENTROID_HPP
