//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder.hpp
 * Non-modifying hierarchy inorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_ALGORITHM_INORDER_HPP
#define BOOST_TREE_ALGORITHM_INORDER_HPP

#include <boost/tree/inorder_iterator.hpp>

namespace boost {
namespace tree {

namespace inorder {

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class MultiwayCursor, class Op>
void for_each_recursive(MultiwayCursor s, Op& f)
{
	if (!s.empty())
		for_each_recursive(s.begin(), f);
	f(*s);
	if (!(++s).empty())
		for_each_recursive(s.begin(), f);
}

/**
 * @brief	Apply a function to every element of a multiway subtree,
 *			in inorder.
 * @param s	A MultiwayTree cursor.
 * @param f	A unary function object.
 * @return	@p f
 *
 * Applies the function object @p f to each element in the @p subtree, using
 * inorder. @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
template <class MultiwayCursor, class Op>
Op for_each(MultiwayCursor s, Op f)
{
	if (!s.empty())
		for_each_recursive(s.begin(), f);
	f(*s);
	if (!(++s).empty())
		for_each_recursive(s.begin(), f);
	return f;
}

/**
 * @brief	Copies the subtree s into t, by traversing s in inorder.
 * @param s	An input cursor.
 * @param t An output cursor.
 * @result	A cursor past t's inorder end, after the copying operation.
 */
template <class InCursor, class OutCursor>
OutCursor copy (InCursor s, OutCursor t)
{
	if (!s.empty())
		copy(s.begin(), t.begin());
	*t = *s;
	if (!(++s).empty())
		copy(s.begin(), (++t).begin());
	return t;
}

/**
 * @brief	 Performs an operation on a subtree, by traversing it in inorder.
 * @param s  An input cursor.
 * @param t  An output cursor.
 * @param op A unary operation.
 * @result	 A cursor past t's inorder end, after the transforming has 
 * 			 finished.
 * 
 * By traversing the input subtree s in inorder, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class InCursor, class OutCursor, class Op>
OutCursor transform (InCursor s, OutCursor t, Op op)
{
	if (!s.empty())
		transform(s.begin(), t.begin(), op);
	*t = op(*s);
	if (!(++s).empty())
		transform(s.begin(), (++t).begin(), op);
	return t;
}


/// Iterators

template <class MultiwayCursor>
iterator<MultiwayCursor, forward_traversal_tag> 
begin(MultiwayCursor c, forward_traversal_tag)
{
	typedef MultiwayCursor cursor;
	std::stack<cursor> s;
 	typename cursor_size<cursor>::type branch = 0;
 	typename cursor_size<cursor>::type i = 0;
	s.push(c);
	while (!s.top().empty()) {
		++i;
		if (!branch && !s.top().end().empty())
			branch = i;
		s.push(s.top().begin());
	}
	return iterator<cursor, forward_traversal_tag>(s, branch);
}

template <class MultiwayCursor>
iterator<MultiwayCursor, forward_traversal_tag> 
end(MultiwayCursor c, forward_traversal_tag)
{
	typedef MultiwayCursor cursor;
	std::stack<cursor> s;
	s.push(c);
	while (!s.top().empty())
		s.push(s.top().end());
	return iterator<cursor, forward_traversal_tag>(s, s.size());
}


} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_ALGORITHM_INORDER_HPP
