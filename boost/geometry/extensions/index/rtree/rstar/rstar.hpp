// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.Index - R*-tree
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RSTAR_RSTAR_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RSTAR_RSTAR_HPP

namespace boost { namespace geometry { namespace index {

struct rstar_tag {};

}}} // namespace boost::geometry::index

#include <boost/geometry/extensions/index/rtree/rstar/insert.hpp>
#include <boost/geometry/extensions/index/rtree/rstar/choose_next_node.hpp>
#include <boost/geometry/extensions/index/rtree/rstar/redistribute_elements.hpp>

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RSTAR_RSTAR_HPP
