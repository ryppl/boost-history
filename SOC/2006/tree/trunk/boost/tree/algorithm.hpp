//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file algorithm.hpp
 * Hierarchy algorithms
 */

#ifndef BOOST_TREE_ALGORITHM_HPP
#define BOOST_TREE_ALGORITHM_HPP

#include <boost/tree/detail/algorithm/cursor/general.hpp>

#include <boost/tree/detail/algorithm/cursor/ascending.hpp>

#include <boost/tree/detail/algorithm/cursor/inorder.hpp>
#include <boost/tree/detail/algorithm/cursor/preorder.hpp>
#include <boost/tree/detail/algorithm/cursor/inorder.hpp>

#include <boost/tree/detail/algorithm/iterator.hpp>
#include <boost/tree/detail/algorithm/iterator.hpp>
#include <boost/tree/detail/algorithm/iterator.hpp>

#include <algorithm>

template <class AscendingCursor>
typename AscendingCursor::size_type parity(AscendingCursor& cur)
{
    return std::distance(cur.parent().begin(), cur);
}

#endif // BOOST_TREE_ALGORITHM_HPP


