//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder.hpp
 * Non-modifying hierarchy preorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_ALGORITHM_PREORDER_HPP
#define BOOST_TREE_ALGORITHM_PREORDER_HPP

#include <boost/tree/preorder_iterator.hpp>

namespace boost {
namespace tree {
	
namespace preorder {

/**
 * @brief	First element of a MultiwayTree in preorder traversal
 * @param t	A MultiwayTree
 * @return	Mutable preorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> begin(MultiwayTree& t)
{
	return iterator<typename MultiwayTree::cursor>(first(t));
}

/**
 * @brief	First element of a MultiwayTree in preorder traversal
 * 			(Alias of cbegin())
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> begin(MultiwayTree const& t)
{
	return cbegin(t);
}

/**
 * @brief	First element of a MultiwayTree in preorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cbegin(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::const_cursor>(cfirst(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in preorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Mutable preorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> end(MultiwayTree& t)
{
	return iterator<typename MultiwayTree::cursor>(last(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in preorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> end(MultiwayTree const& t)
{
	return cend(t);
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in preorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cend(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::const_cursor>(clast(t));
}

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_ALGORITHM_PREORDER_HPP
