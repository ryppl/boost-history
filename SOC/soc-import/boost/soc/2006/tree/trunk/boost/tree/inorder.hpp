//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
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
