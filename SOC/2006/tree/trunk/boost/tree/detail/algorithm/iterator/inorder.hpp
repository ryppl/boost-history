//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder.hpp
 * Non-modifying hierarchy inorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_INORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_INORDER_HPP

#include <boost/tree/detail/iterator/inorder.hpp>
#include <boost/tree/detail/algorithm/cursor/inorder.hpp>

namespace boost {
namespace tree {

namespace inorder {

#include <boost/tree/detail/algorithm/iterator/bidirectional.hpp>

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_INORDER_HPP
