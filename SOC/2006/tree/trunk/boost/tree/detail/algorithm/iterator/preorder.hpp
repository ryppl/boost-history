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

#ifndef BOOST_TREE_ALGORITHM_PREORDER_HPP
#define BOOST_TREE_ALGORITHM_PREORDER_HPP

#include <boost/tree/preorder_iterator.hpp>

namespace boost {
namespace tree {
	
namespace preorder {

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class Cursor, class Op>
void for_each_recursive(Cursor s, Op& f)
{
	f(*s);
	if (!s.empty())
		for_each_recursive(s.begin(), f);
	if (!(++s).empty())
		for_each_recursive(s.begin(), f);
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
	f(*s);
	if (!s.empty())
		for_each_recursive(s.begin(), f);
	if (!(++s).empty())
		for_each_recursive(s.begin(), f);
	return f;
}

/**
 * @brief	Copies the subtree s into t, by traversing s in preorder.
 * @param s	An input cursor.
 * @param t An output cursor.
 * @result	A cursor past t's preorder end, after the copying operation.
 */
// TODO: Should work with root() instead of root().begin() (same for in- and postorder, )
// plus a couple of other algorithms
template <class InCursor, class OutCursor>
OutCursor copy (InCursor s, OutCursor t)
{
	*t = *s;
	if (!s.empty())
		copy(s.begin(), t.begin());
	if (!(++s).empty())
		copy(s.begin(), (++t).begin());
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
	*t = op(*s);
	if (!s.empty())
		transform(s.begin(), t.begin(), op);
	if (!(++s).empty())
		transform(s.begin(), (++t).begin(), op);
	return t;
}

/// Iterators

template <class Cursor>
iterator<Cursor, forward_traversal_tag> 
begin(Cursor c, forward_traversal_tag)
{
	std::stack<Cursor> s;

	s.push(c);
	s.push(c.begin());
	return iterator<Cursor, forward_traversal_tag>(s);	
}

template <class Cursor>
iterator<Cursor, forward_traversal_tag> 
end(Cursor c, forward_traversal_tag)
{
	std::stack<Cursor> s;

	s.push(c);
	return iterator<Cursor, forward_traversal_tag>(s);
}

#include <boost/tree/algorithm/iterator/bidirectional.hpp>

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_ALGORITHM_PREORDER_HPP
