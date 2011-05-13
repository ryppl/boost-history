// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.Index - R*-tree ChooseNextNode algorithm
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RSTAR_CHOOSE_NEXT_NODE_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RSTAR_CHOOSE_NEXT_NODE_HPP

#include <algorithm>

#include <boost/geometry/algorithms/expand.hpp>

#include <boost/geometry/extensions/index/algorithms/area.hpp>
#include <boost/geometry/extensions/index/algorithms/overlap.hpp>
#include <boost/geometry/extensions/index/algorithms/union_area.hpp>

#include <boost/geometry/extensions/index/rtree/node.hpp>
#include <boost/geometry/extensions/index/rtree/visitors/is_leaf.hpp>

namespace boost { namespace geometry { namespace index {

namespace detail { namespace rtree { namespace visitors {

namespace detail {

template <typename Value, typename Box>
class choose_next_node<Value, Box, rstar_tag>
{
    typedef typename rtree::node<Value, Box, rstar_tag>::type node;
    typedef typename rtree::internal_node<Value, Box, rstar_tag>::type internal_node;
    typedef typename rtree::leaf<Value, Box, rstar_tag>::type leaf;

    typedef typename rtree::elements_type<internal_node>::type children_type;

    typedef typename index::default_area_result<Box>::type area_type;
    typedef typename index::default_overlap_result<Box>::type overlap_type;

public:
    template <typename Indexable>
    static inline size_t apply(internal_node & n, Indexable const& indexable)
    {
        children_type & children = rtree::elements(n);
        assert(!children.empty());
        
        visitors::is_leaf<Value, Box, rstar_tag> ilv;
        rtree::apply_visitor(ilv, *children.front().second);

        if ( ilv.result )
            return choose_by_minimum_overlap_cost(children, indexable);
        else
            return choose_by_minimum_area_cost(children, indexable);
    }

private:
    template <typename Indexable>
    static inline size_t choose_by_minimum_overlap_cost(children_type const& children, Indexable const& indexable)
    {
        size_t children_count = children.size();

        // choose index with smallest overlap change value, or area change or smallest area
        size_t choosen_index = 0;
        overlap_type smallest_overlap_diff = std::numeric_limits<overlap_type>::max();
        area_type smallest_area_diff = std::numeric_limits<area_type>::max();
        area_type smallest_area = std::numeric_limits<area_type>::max();

        // for each child node
        for (size_t i = 0 ; i < children_count ; ++i )
        {
            typedef typename children_type::value_type child_type;
            child_type const& ch_i = children[i];

            Box box_exp(ch_i.first);
            // calculate expanded box fo node ch_i
            geometry::expand(box_exp, indexable);

            // calculate area and area diff
            area_type area = index::area(ch_i.first);
            area_type area_diff = index::area(box_exp) - area;

            overlap_type overlap = 0;
            overlap_type overlap_exp = 0;

            // calculate overlap
            for ( size_t j = 0 ; j < children_count ; ++j )
            {
                if ( i != j )
                {
                    child_type const& ch_j = children[j];

                    overlap += index::overlap(ch_i.first, ch_j.first);
                    overlap_exp += index::overlap(box_exp, ch_j.first);
                }
            }

            overlap_type overlap_diff = overlap_exp - overlap;

            // update result
            if ( overlap_diff < smallest_overlap_diff ||
                 ( overlap_diff == smallest_overlap_diff && area_diff < smallest_area_diff ) ||
                 ( area_diff == smallest_area_diff && area < smallest_area ) )
            {
                smallest_overlap_diff = overlap_diff;
                smallest_area_diff = area_diff;
                smallest_area = area;
                choosen_index = i;
            }
        }

        return choosen_index;
    }

    template <typename Indexable>
    static inline size_t choose_by_minimum_area_cost(children_type const& children, Indexable const& indexable)
    {
        size_t children_count = children.size();

        // choose index with smallest area change or smallest area
        size_t choosen_index = 0;
        area_type smallest_area_diff = std::numeric_limits<area_type>::max();
        area_type smallest_area = std::numeric_limits<area_type>::max();

        // caculate areas and areas of all nodes' boxes
        for ( size_t i = 0 ; i < children_count ; ++i )
        {
            typedef typename children_type::value_type child_type;
            child_type const& ch_i = children[i];

            Box box_exp(ch_i.first);
            geometry::expand(box_exp, indexable);

            area_type area = index::area(box_exp);
            area_type area_diff = area - index::area(ch_i.first);

            if ( area_diff < smallest_area_diff ||
                ( area_diff == smallest_area_diff && area < smallest_area ) )
            {
                smallest_area_diff = area_diff;
                smallest_area = area;
                choosen_index = i;
            }
        }

        return choosen_index;
    }
};

} // namespace detail

}}} // namespace detail::rtree::visitors

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RSTAR_CHOOSE_NEXT_NODE_HPP
