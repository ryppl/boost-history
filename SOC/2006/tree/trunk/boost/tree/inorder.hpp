//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder.hpp
 * Inorder traversal algorithms for cursors
 */
//TODO:
// Concept checks: MultiwayTree, parent?
// Optimise for trees such as binary_tree with their own inorder begin() members!

#ifndef BOOST_TREE_INORDER_HPP
#define BOOST_TREE_INORDER_HPP

#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_categories.hpp>


namespace boost {
namespace tree {

namespace inorder {

/** \addtogroup traversal */
/*\@{*/

/**
 * @brief	Inorder successor
 * @param c	MultiwayCursor to be set to its inorder successor
 * @ingroup	traversal
 */
template <class MultiwayCursor>
inline void forward(MultiwayCursor& c)
{
	if (!(++c).empty()) {
		while (!c.begin().empty())
			c = c.begin();
		c = c.begin();
		return;
	}
	
	while (c.parity()) 
		c = c.parent();
	if (c.parent().begin() != c) // Root? Would be nice to get rid of.
		c = c.parent(); // Shoot (root's parent)
	return;
}

/**
 * @brief	Inorder successor
 * @param c	A cursor
 * @return	Inorder successor of @a c
 * @ingroup	traversal
 */
template <class MultiwayCursor>
inline MultiwayCursor next(MultiwayCursor c)
{
	forward(c);
	return c;
}

/**
 * @brief	Inorder predecessor
 * @param c	MultiwayCursor to be set to its inorder predecessor
 */
template <class MultiwayCursor>
inline void back(MultiwayCursor& c)
{
    	if (!c.empty()) {
		while (!c.end().empty())
			c = c.end();
		c = c.begin();
		return;
	}
	while (!c.parity())
		c = c.parent();
	--c;
	return;
}

/**
 * @brief	Inorder predecessor
 * @param c	MultiwayCursor
 * @return	Inorder predecessor of @a c
 */
template <class MultiwayCursor>
inline MultiwayCursor prior(MultiwayCursor c)
{
	back(c);
	return c;
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::first())
 * @param t	A MultiwayTree
 * @return	Mutable cursor to the first element of @a t in inorder traversal
 */
template <class MultiwayTree>
typename MultiwayTree::cursor first(MultiwayTree& t)
{
	typename MultiwayTree::cursor c = t.root();
	while (!c.empty())
		c = c.begin();
	return c;
}

/**
 * @brief	First element of a MultiwayTree in inorder 
 * 			traversal (Alias of cfirst(); equivalent to postorder::first())
 * @param t	A MultiwayTree
 * @return	Read-only cursor to the first element of @a t in inorder traversal
 */
template <class MultiwayTree>
typename MultiwayTree::const_cursor first(MultiwayTree const& t)
{
	typename MultiwayTree::const_cursor c = t.root();
	while (!c.empty())
		c = c.begin();
	return c;
}

/**
 * @brief	First element of a MultiwayTree in inorder 
 * 			traversal (equivalent to postorder::cfirst())
 * @param t	A MultiwayTree
 * @return	Read-only cursor to the first element of @a t in inorder traversal
 */
template <class MultiwayTree>
typename MultiwayTree::const_cursor cfirst(MultiwayTree const& t)
{
	typename MultiwayTree::const_cursor c = t.root();
	while (!c.empty())
		c = c.begin();
	return c;
}

/**
 * @brief	One position past the last element of a MultiwayTree in 
 * 			inorder traversal
 * @param t	A MultiwayTree
 * @return	Mutable cursor one position past the last element of @a t in inorder
 * 			traversal
 */
template <class MultiwayTree>
typename MultiwayTree::cursor last(MultiwayTree& t)
{
	return t.shoot();
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal (Alias of clast())
 * @param t	A MultiwayTree
 * @return	Read-only cursor one position past the last element of @a t in 
 * 			inorder traversal
 */
template <class MultiwayTree>
typename MultiwayTree::const_cursor last(MultiwayTree const& t)
{
	return t.cshoot();
}

/**
 * @brief	One position past the last element of a MultiwayTree
 * 			in inorder traversal
 * @param t	A tree
 * @return	Read-only cursor one position past the last element of @a t in 
 * 			inorder traversal
 */
template <class MultiwayTree>
typename MultiwayTree::const_cursor clast(MultiwayTree const& t)
{
	return t.cshoot();
}

/*\@}*/

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_HPP
