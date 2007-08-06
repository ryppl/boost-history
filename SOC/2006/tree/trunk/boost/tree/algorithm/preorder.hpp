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
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class Cursor, class Op>
void for_each_recursive(Cursor s, Op& f)
{
	f(*s);
	if (!s.empty())
		for_each_recursive(s.begin(), f);
	if (!(++s).empty())
		for_each_recursive(s.begin(), f);
}

/**
 * @brief	Apply a function to every element of a subtree, in preorder.
 * @param s	A cursor.
 * @param f	A unary function object.
 * @return	@p f
 *
 * Applies the function object @p f to each element in the @p subtree, using  
 * preorder. @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
template <class Cursor, class Op>
Op for_each(Cursor s, Op f)
{
	f(*s);
	if (!s.empty())
		for_each_recursive(s.begin(), f);
	if (!(++s).empty())
		for_each_recursive(s.begin(), f);
	return f;
}

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
