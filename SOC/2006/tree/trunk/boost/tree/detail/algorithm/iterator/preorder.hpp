//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder.hpp
 * Non-modifying hierarchy preorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_PREORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_PREORDER_HPP

#include <boost/tree/detail/iterator/preorder.hpp>
#include <boost/tree/detail/algorithm/cursor/preorder.hpp>

namespace boost {
namespace tree {
    
namespace preorder {

#include <boost/tree/detail/algorithm/iterator/_order.hpp>

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_PREORDER_HPP
