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
	if (!c.empty()) { // Left.
		c.to_begin();
		return;
	}
	if (!(++c).empty()) { // Right.
		c.to_begin();
		return;
	}

	while (c.parity())
		c.to_parent();
	if (!(++c).empty()) {
		c.to_begin();
		return;
	}
	
	--c;
	while (!c.empty()) {
		c.to_end();
		if (c.empty())
			--c;
	}
	if (!c.parity()) 
		++c;
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
	c.to_parent();
	if (c.parity()) {
		--c;
		while (!c.empty())
			c.to_end();
		return;
	}
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
 * @brief	First element of a tree in preorder traversal
 * @param t	A tree
 * @return	Mutable cursor to the first element of @a t in preorder traversal
 */
template <class Tree>
typename Tree::cursor first(Tree& t)
{
	return t.root().begin();
}

/**
 * @brief	First element of a tree in preorder traversal
 * 			(Alias of cfirst())
 * @param t	A tree
 * @return	Read-only cursor to the first element of @a t in preorder traversal
 */
template <class Tree>
typename Tree::const_cursor first(Tree const& t)
{
	return t.croot().begin();
}

/**
 * @brief	First element of a tree in preorder traversal
 * @param t	A tree
 * @return	Read-only cursor to the first element of @a t in preorder traversal
 */
template <class Tree>
typename Tree::const_cursor cfirst(Tree const& t)
{
	return t.croot().begin();
}

/**
 * @brief	One position past the last element of a tree in preorder 
 * 			traversal
 * @param t	A tree
 * @return	Mutable cursor one position past the last element of @a t in preorder
 * 			traversal
 */
template <class Tree>
typename Tree::cursor last(Tree& t)
{
	typename Tree::cursor c = t.shoot();
	--c;	
	while (!c.empty()) {
		c.to_end();
		if (c.empty())
			--c;
	}
	return ++c;
}

/**
 * @brief	One position past the last element of a tree in 
 * 			preorder traversal (Alias of clast())
 * @param t	A tree
 * @return	Read-only cursor one position past the last element of @a t in 
 * 			preorder traversal
 */
template <class Tree>
typename Tree::const_cursor last(Tree const& t)
{
	typename Tree::const_cursor c = t.cshoot();
	--c;	
	while (!c.empty()) {
		c.to_end();
		if (c.empty())
			--c;
	}
	return ++c;
}

/**
 * @brief	const_cursor to one position past the last element of a tree in 
 * 			preorder traversal
 * @param t	A tree
 * @return	Read-only cursor one position past the last element of @a t in 
 * 			preorder traversal
 */
template <class Tree>
typename Tree::const_cursor clast(Tree const& t)
{
	typename Tree::const_cursor c = t.cshoot();
	--c;	
	while (!c.empty()) {
		c.to_end();
		if (c.empty())
			--c;
	}
	return ++c;
}

/*\@}*/

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_PREORDER_HPP
