// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder.hpp
 * Inorder traversal algorithms for cursors
 */

#ifndef BOOST_TREE_INORDER_HPP
#define BOOST_TREE_INORDER_HPP

namespace boost {
namespace tree {


namespace inorder {

/**
 * @brief	Inorder successor
 * @param c	MultiwayCursor that to be set to its inorder successor
 */
template <class MultiwayCursor>
inline void forward(MultiwayCursor& c) // ++ (go right, then left)
{
	if ((++c).has_child()) {
		while (c.begin().has_child())
			c = c.begin();
		c = c.begin();
		return;
	}
	
// TODO: unfortunately, this isn't the same as
// while (c.parity()) 
// atm due to some dirty hacking required
// so root(), shoot() and (inorder) begin() work at the same time
// Need to change that; see binary_tree and (const_)tree_cursor::parent()
	while (c.parent().begin() == c) 
		c = c.parent();
	if (c.parent().end() != c)
		c = c.parent();
	return;
}

/**
 * @brief	Inorder predecessor
 * @param c	MultiwayCursor that to be set to its inorder predecessor
 */
template <class MultiwayCursor>
inline void back(MultiwayCursor& c) // -- (go left, then right)
{
    //	typename cursor_size<MultiwayCursor>::type par = 0;
    	if (c.has_child()) {
		//par = 1;
		while (c.end().has_child())
			c = c.end();
		c = c.begin();
		return;
	}
	while (!c.parity()) // && (c != c.parent()))
		c = c.parent();
	c = c.parent().begin();
	return;
}

// TODO: begin, end (of MultiwayTree)
// Concept checks: MultiwayTree, parent?

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_HPP
