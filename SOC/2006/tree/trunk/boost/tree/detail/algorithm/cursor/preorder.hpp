//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder.hpp
 * Non-modifying hierarchy preorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_PREORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_PREORDER_HPP

#include <boost/tree/detail/iterator/preorder.hpp>

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
		if (c.is_root())
			return;
		
		c.to_parent();
		if (!c.parity()) {
			if (c.is_root())
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
	if (!c.is_root()) { // Not root
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

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class Cursor, class Op>
void for_each_recursive(Cursor s, Op& f)
{
	Cursor t = s;
	t.to_end();
	s.to_begin();
	f(*s);
	do
		if (!s.empty())
			for_each_recursive(s, f);
	while (s++ != t);
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
	Cursor t = s.end();
	s.to_begin();
	f(*s);
	do
		if (!s.empty())
			for_each_recursive(s, f);
	while (s++ != t);
		
	return f;
}

/**
 * @brief	Copies the subtree s into t, by traversing s in preorder.
 * @param s	An input cursor.
 * @param t An output cursor.
 * @result	A cursor past t's preorder end, after the copying operation.
 */
template <class InCursor, class OutCursor>
OutCursor copy (InCursor s, OutCursor t)
{
	InCursor r = s.end();
	*t.to_begin() = *s.to_begin();
	do {
		if (!s.empty())
			copy(s, t);
		++t;
	} while (s++ != r);

	return t;
}

/**
 * @brief	 Performs an operation on a subtree, by traversing it in preorder.
 * @param s  An input cursor.
 * @param t  An output cursor.
 * @param op A unary operation.
 * @result	 A cursor past t's preorder end, after the transforming has 
 * 			 finished.
 * 
 * By traversing the input subtree s in preorder, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class InCursor, class OutCursor, class Op>
OutCursor transform (InCursor s, OutCursor t, Op op)
{
	InCursor r = s.end();
	*t.to_begin() = op(*s.to_begin());
	do {
		if (!s.empty())
			transform(s, t, op);
		++t;
	} while (s++ != r);

	return t;
}

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_CURSOR_PREORDER_HPP
