//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder.hpp
 * Preorder traversal algorithms for cursors
 */
// TODO: Concept checks: Non-MultiwayTree or forest, parent?

#ifndef BOOST_TREE_PREORDER_HPP
#define BOOST_TREE_PREORDER_HPP

#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_categories.hpp>

namespace boost {
namespace tree {

namespace preorder {

/** \addtogroup traversal */
/*\@{*/

/**
 * @brief	Preorder successor
 * @param c	Cursor to be set to its preorder successor
 */
template <class Cursor>
inline void forward(Cursor& c)
{
	// If we have a left child, go there.
	if (!c.empty()) {
		c.to_begin();
		return;
	}
	
	// No left child. So if we have a right child, go there.
	if (!(++c).empty()) {
		c.to_begin();
		return;
	}
	
	// (Here's where we'd need to check if this is the end().)
	
	// No children at all.
	// As we've already visited all the ancestors, we'll move upwards until
	// we find an ancestor that has a right child.
	while (true) { // Doesn't work with subtrees!
		if (!c.parity() && (c != c.parent().begin())) { // Root
			c.to_parent();
			return;
		}
		c.to_parent();
		if (!c.parity()) {
			if (c != c.parent().begin()) // Root?
				return;
			if (!(++c).empty()) {
				c.to_begin();
				return;
			}
		}
	}
	return;
}

/**
 * @brief	Preorder successor
 * @param c	A cursor
 * @return	Preorder successor of @a c
 */
template <class Cursor>
inline Cursor next(Cursor c)
{
	forward(c);
	return c;
}

/**
 * @brief	Preorder predecessor
 * @param c	Cursor to be set to its preorder predecessor
 */
template <class Cursor>
inline void back(Cursor& c)
{
	if (!(!c.parity() && (c != c.parent().begin()))) { // Not root
		c.to_parent();
		
		// If this is a left child, just move to its parent.
		if (!c.parity()) {
			c.to_parent();
			c.to_begin();
			return;
		}
		
		// So this is a right child.
		--c;
	}
	
	// Same for root (=end) and right children:
	if (!c.empty())
		while (!c.empty())
			if (!c.end().empty())
				c.to_end();
			else
				c.to_begin();
	return;
}

/**
 * @brief	Preorder predecessor
 * @param c	A cursor
 * @return	Preorder predecessor of @a c
 */
template <class Cursor>
inline Cursor prior(Cursor c)
{
	back(c);
	return c;
}

/**
 * @brief	First element of a subtree in preorder traversal
 * @param c	A cursor
 * @return	Cursor to the first element of @a c in preorder traversal
 */
template <class Cursor>
Cursor first(Cursor c)
{
	return c.begin();
}

/**
 * @brief	One position past the last element of a subtree in preorder 
 * 			traversal
 * @param c	A cursor
 * @return	Cursor one position past the last element of @a c in preorder
 * 			traversal
 */
template <class Cursor>
Cursor last(Cursor c)
{
	return c;
}

/*\@}*/

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_PREORDER_HPP
