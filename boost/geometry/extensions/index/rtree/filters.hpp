// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - rtree queries filters implementation
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_FILTERS_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_FILTERS_HPP

#include <boost/static_assert.hpp>

#include <boost/geometry/extensions/index/rtree/rtree.hpp>
#include <boost/geometry/extensions/index/filters/spacial_filter.hpp>
#include <boost/geometry/extensions/index/filters/nearest_filter.hpp>

namespace boost { namespace geometry { namespace index { namespace filters {

template <typename Value, typename Translator, typename Box>
class spatial_filter< index::rtree<Value, Translator, Box> >
{
public:
    typedef typename std::deque<Value>::iterator iterator;
    typedef typename std::deque<Value>::const_iterator const_iterator;

    spatial_filter(index::rtree<Value, Translator, Box> const& rtree, Box const& b)
    {
        m_result = rtree.find(b);
    }

    iterator begin() { return m_result.begin(); }
    iterator end() { return m_result.end(); }
    const_iterator begin() const { return m_result.begin(); }
    const_iterator end() const { return m_result.end(); }

private:
    std::deque<Value> m_result;
};

namespace detail {

template <typename Box, size_t D>
struct sphere_to_min_max
{
    BOOST_STATIC_ASSERT(0 < D);

    static void apply(
        typename traits::point_type<Box>::type & min_point,
        typename traits::point_type<Box>::type & max_point,
        typename traits::point_type<Box>::type const& centre,
        typename traits::coordinate_type<typename traits::point_type<Box>::type>::type const& radius
        )
    {
        geometry::set<D - 1>(min_point, geometry::get<D - 1>(centre) - radius);
        geometry::set<D - 1>(max_point, geometry::get<D - 1>(centre) + radius);

        sphere_to_min_max<Box, D - 1>::apply(min_point, max_point, centre, radius);
    }
};

template <typename Box>
struct sphere_to_min_max<Box, 1>
{
    static void apply(
        typename traits::point_type<Box>::type & min_point,
        typename traits::point_type<Box>::type & max_point,
        typename traits::point_type<Box>::type const& centre,
        typename traits::coordinate_type<typename traits::point_type<Box>::type>::type const& radius
        )
    {
        geometry::set<0>(min_point, geometry::get<0>(centre) - radius);
        geometry::set<0>(max_point, geometry::get<0>(centre) + radius);
    }
};

} // namespace detail

// TODO
// implement final version of nearest filter
// range should be sorted (for Boxes - by closest corner)

template <typename Value, typename Translator, typename Box>
class nearest_filter< index::rtree<Value, Translator, Box> >
{
public:
    typedef typename std::deque<Value>::iterator iterator;
    typedef typename std::deque<Value>::const_iterator const_iterator;

    template <typename Point>
    nearest_filter(
        index::rtree<Value, Translator, Box> const& rtree,
        Point const& box_centre,
        typename traits::coordinate_type<Point>::type const& radius
        )
    {
    	typename traits::point_type<Box>::type pmin, pmax;
        detail::sphere_to_min_max<
            Box,
            traits::dimension<typename traits::point_type<Box>::type>::value
        >::apply(pmin, pmax, box_centre, radius);

        m_result = rtree.find(Box(pmin, pmax));
    }

    iterator begin() { return m_result.begin(); }
    iterator end() { return m_result.end(); }
    const_iterator begin() const { return m_result.begin(); }
    const_iterator end() const { return m_result.end(); }

private:
    std::deque<Value> m_result;
};

}}}} // namespace boost::geometry::index::filters

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_FILTERS_HPP
