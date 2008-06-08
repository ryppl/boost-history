//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file postorder.hpp
 * Postorder traversal algorithms for cursors
 */
// TODO: Concept checks: Non-MultiwayTree or forest, parent?

#ifndef BOOST_TREE_POSTORDER_HPP
#define BOOST_TREE_POSTORDER_HPP

#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_categories.hpp>

namespace boost {
namespace tree {

namespace postorder {

/** \addtogroup traversal */
/*\@{*/

/**
 * @brief	Postorder successor
 * @param c	Cursor to be set to its postorder successor
 */
template <class Cursor>
inline void forward(Cursor& c)
{
	c.to_parent();

	if (c.parity()) { // Right child? Return parent.
		--c;
		return;
	}
	
	if (c.parent().begin() != c) // Root?
		return;
	
	// Left child.
	++c;
	while (!c.empty()) {
		c.to_begin();
		if (c.empty())
			++c;
	}
	if (c.parity())
		--c;
	return;
}

/**
 * @brief	Postorder successor
 * @param c	A cursor
 * @return	Postorder successor of @a c
 */
template <class Cursor>
inline Cursor next(Cursor c)
{
	forward(c);
	return c;
}

/**
 * @brief	Postorder predecessor
 * @param c	Cursor to be set to its postorder predecessor
 */
template <class Cursor>
inline void back(Cursor& c)
{
	if (!c.parity() && (c.parent().begin() != c)) { // Root?
		c.to_begin();
		return;
	}
	
	if (!(++c).empty()) { // Right
		c.to_begin();
		return;
	}
	if (!(--c).empty()) { // Left
		c.to_begin();
		return;
	}
	
	// Move up in the hierarchy until we find a descendant that has a right
	// child (which is what we'll return) or we land at root.
	while (true) { // revisit
		c.to_parent();
		if (c.parity())
			if (!(--c).empty()) {
				c.to_begin();
				return;
			}
	}
	return;
}

/**
 * @brief	Postorder predecessor
 * @param c	A cursor
 * @return	Postorder predecessor of @a c
 */
template <class Cursor>
inline Cursor prior(Cursor c)
{
	back(c);
	return c;
}

/**
 * @brief	First element of a subtree in postorder traversal
 * @param c	A cursor
 * @return	Cursor to the first element of @a c in postorder traversal
 */
template <class Cursor>
Cursor first(Cursor c)
{
	while (true)
		if (!c.empty())
			c.to_begin();
		else if (!(++c).empty())
			c.to_begin();
		else
			return --c;
}

/**
 * @brief	One position past the last element of a subtree in postorder 
 * 			traversal
 * @param c	A cursor
 * @return	Cursor one position past the last element of @a c in 
 * 			postorder traversal
 */
template <class Cursor>
Cursor last(Cursor c)
{
	return c;
}

/*\@}*/

} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_POSTORDER_HPP
