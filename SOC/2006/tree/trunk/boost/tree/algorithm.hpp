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


#include <boost/tree/search.hpp>
#include <boost/tree/traversal.hpp>

#include <algorithm>

template <class AscendingCursor>
typename AscendingCursor::size_type parity(AscendingCursor& cur)
{
	return std::distance(cur.parent().begin(), cur);
}

#endif // BOOST_TREE_ALGORITHM_HPP


