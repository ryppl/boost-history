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
	c = c.parent();

	if (c.parity()) { // Right child? Return parent.
		--c;
		return;
	}
	
	if (/*!c.parity() &&*/ (c.parent().begin() != c)) // Root?
		return;
	
	// Left child.
	++c;
	while (!c.empty()) {
		c = c.begin();
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
		c = c.begin();
		return;
	}
	if (!(++c).empty()) { // Right
		c = c.begin();
		return;
	}
	if (!(--c).empty()) { // Left
		c = c.begin();
		return;
	}
	
	while (true) { // revisit
	c = c.parent();
	if (c.parity())
		if (!(--c).empty()) {
			c = c.begin();
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
 * @brief	First element of a tree in postorder traversal
 * 			(equivalent to inorder::first())
 * @param t	A tree
 * @return	Mutable cursor to the first element of @a t in postorder traversal
 */
template <class Tree>
typename Tree::cursor first(Tree& t)
{
	typename Tree::cursor c = t.root();
	while (!c.empty())
		c = c.begin();
	return c;
}

/**
 * @brief	First element of a tree in postorder traversal
 * 			(Alias of cfirst(); equivalent to inorder::first())
 * @param t	A tree
 * @return	Read-only cursor to the first element of @a t in postorder traversal
 */
template <class Tree>
typename Tree::const_cursor first(Tree const& t)
{
	typename Tree::const_cursor c = t.root();
	while (!c.empty())
		c = c.begin();
	return c;
}

/**
 * @brief	First element of a tree in postorder traversal
 * 			(equivalent to inorder::cfirst())
 * @param t	A tree
 * @return	Read-only cursor to the first element of @a t in postorder traversal
 */
template <class Tree>
typename Tree::const_cursor cfirst(Tree const& t)
{
	typename Tree::const_cursor c = t.root();
	while (!c.empty())
		c = c.begin();
	return c;
}

/**
 * @brief	One position past the last element of a tree in postorder 
 * 			traversal
 * @param t	A tree
 * @return	Mutable cursor one position past the last element of @a t in 
 * 			postorder traversal
 */
template <class Tree>
typename Tree::cursor last(Tree& t)
{
	return t.root();
}

/**
 * @brief	One position past the last element of a tree in 
 * 			postorder traversal (Alias of clast())
 * @param t	A tree
 * @return	Read-only cursor one position past the last element of @a t in 
 * 			postorder traversal
 */
template <class Tree>
typename Tree::const_cursor last(Tree const& t)
{
	return t.croot();
}

/**
 * @brief	One position past the last element of a tree in 
 * 			postorder traversal
 * @param t	A tree
 * @return	Read-only cursor one position past the last element of @a t in 
 * 			postorder traversal
 */
template <class Tree>
typename Tree::const_cursor clast(Tree const& t)
{
	return t.croot();
}

/*\@}*/

} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_POSTORDER_HPP
