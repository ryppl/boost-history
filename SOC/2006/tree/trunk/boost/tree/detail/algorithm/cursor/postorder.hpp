//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file postorder.hpp
 * Non-modifying hierarchy postorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_POSTORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_POSTORDER_HPP

#include <boost/tree/detail/iterator/postorder.hpp>

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

	if (c.is_root())
		return;

	if (c.parity()) { // Right child? Return parent.
		--c;
		return;
	}
		
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
	if (c.is_root()) { // Root?
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
	while (!c.is_root()) { // revisit
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
	s.to_begin();
	do
		if (!s.empty())
			for_each_recursive(s, f);
	while (s++ != t.end());
	f(*t.to_begin());
}

/**
 * @brief	Apply a function to every element of a subtree, in postorder.
 * @param s	A cursor.
 * @param f	A unary function object.
 * @return	@p f
 *
 * Applies the function object @p f to each element in the subtree @p s, using 
 * postorder. @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
//[postorder_for_each
template <class Cursor, class Op>
Op for_each(Cursor s, Op f)
{
	Cursor t = s;
	s.to_begin();
	do
		if (!s.empty())
			for_each_recursive(s, f);
	while (s++ != t.end());
	f(*t.to_begin());

	return f;
}
//]

/**
 * @brief	Copies the subtree s into t, by traversing s in postorder.
 * @param s	An input cursor.
 * @param t An output cursor.
 * @result	A cursor past t's postorder end, after the copying operation.
 */
template <class InCursor, class OutCursor>
OutCursor copy (InCursor s, OutCursor t)
{
	InCursor r = s;
	s.to_begin();
	t.to_begin();
	do {
		if (!s.empty())
			copy(s, t);
		++t;
	} while (s++ != r.end());
	*t = *r.to_begin();
	return t;
}

/**
 * @brief	 Performs an operation on a subtree, by traversing it in postorder.
 * @param s  An input cursor.
 * @param t  An output cursor.
 * @param op A unary operation.
 * @result	 A cursor past t's postorder end, after the transforming has
 * 			 finished.
 * 
 * By traversing the input subtree s in postorder, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class InCursor, class OutCursor, class Op>
OutCursor transform (InCursor s, OutCursor t, Op op)
{
	InCursor r = s;
	s.to_begin();
	t.to_begin();
	do {
		if (!s.empty())
			copy(s, t);
		++t;
	} while (s++ != r.end());
	*t = op(*r.to_begin());
	return t;
}

} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_CURSOR_POSTORDER_HPP
