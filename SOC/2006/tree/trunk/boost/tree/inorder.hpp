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
		while (!c.to_begin().empty());
		return;
	}
	
	while (c.parity()) // Doesn't work with subtrees whose root's parity != 0
		c.to_parent();
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
		while (!c.to_end().empty());
		--c;
		return;
	}
	while (!c.parity())
		c.to_parent();
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
 * @brief	First element of a Multiway subtree in inorder traversal
 * @param c	A MultiwayCursor
 * @return	Cursor to the first element of @a c in inorder traversal
 */
template <class MultiwayCursor>
MultiwayCursor first(MultiwayCursor c)
{
	while (!c.empty())
		c.to_begin();
	return c;
}

/**
 * @brief	One position past the last element of a Multiway subtree in 
 * 			inorder traversal
 * @param c	A MultiwayCursor
 * @return	Cursor one position past the last element of @a c in inorder
 * 			traversal
 */
template <class MultiwayCursor>
MultiwayCursor last(MultiwayCursor c)
{
	return c;
}

/*\@}*/

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_HPP
