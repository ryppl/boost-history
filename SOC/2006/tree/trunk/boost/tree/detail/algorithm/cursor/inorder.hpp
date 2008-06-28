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

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_INORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_INORDER_HPP

#include <boost/tree/detail/iterator/inorder.hpp>

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
	
	while (c.parity() && !c.is_root())
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
	
	while (!c.parity() && !c.is_root())
		c.to_parent();
	if (!c.is_root())
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

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class MultiwayCursor, class Op>
void for_each_recursive(MultiwayCursor s, Op& f)
{
	MultiwayCursor t = s.end();

	for (s.to_begin(); s!=t; ++s) {
		if (!s.empty())
			for_each_recursive(s, f);
		f(*s);
	}
	if (!t.empty())
		for_each_recursive(t, f);
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
	MultiwayCursor t = s.end();

	for (s.to_begin(); s!=t; ++s) {
		if (!s.empty())
			for_each_recursive(s, f);
		f(*s);
	}
	if (!t.empty())
		for_each_recursive(t, f);
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
	InCursor r = s.end();

	s.to_begin();
	t.to_begin();
	
	while (s!=r) {
		if (!s.empty())
			copy(s, t);
		*++t=*++s;
	}
	if (!r.empty())
		copy(r, t);
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
	InCursor r = s.end();

	s.to_begin();
	t.to_begin();
	
	while (s!=r) {
		if (!s.empty())
			transform(s, t, op);
		*++t=op(*++s);
	}
	if (!r.empty())
		transform(r, t, op);
	return t;
}

/// Search

/**
 * @brief		Finds the first position in a multiway subtree in which @a val 
 * 				could be inserted without changing the ordering, using < (less
 * 				than) for comparisons.
 * @param x		The subtree's root
 * @param val	The search term
 * @return		A multiway cursor pointing to the first element not less than 
 *				@a val, or @x if every element in the subtree is less than 
 * 				@a val.
 */
template <class MultiwayCursor, class T>
MultiwayCursor lower_bound(MultiwayCursor x, T const& val)
{
	MultiwayCursor y = x;
	while (!x.empty()) {
		x = std::lower_bound(x.begin(), x.end(), val);
		if (x.parity() == 0)
			y = x;
	}
	return y;
}

/**
 * @brief		Finds the first position in a multiway subtree in which @a val 
 * 				could be inserted without changing the ordering, using @a cmp 
 * 				for comparisons.
 * @param x		The subtree's root
 * @param val	The search term
 * @param cmp	The comparison functor
 * @return		A multiway cursor pointing to the first element not less than 
 *				@a val, or @x if every element in the subtree is less than 
 * 				@a val.
 */
template <class MultiwayCursor, class T, class Cmp>
MultiwayCursor lower_bound(MultiwayCursor x, T const& val, Cmp cmp)
{
	MultiwayCursor y = x;
	while (!x.empty()) {
		x = std::lower_bound(x.begin(), x.end(), val, cmp);
		if (x.parity() == 0)
			y = x;
	}
	return y;
}

/**
 * @brief		Finds the last position in a multiway subtree in which @a val 
 * 				could be inserted without changing the ordering, using < (less
 * 				than) for comparisons.
 * @param x		The subtree's root
 * @param val	The search term
 * @return		A multiway cursor pointing to the first element greater than 
 *				@a val, or @x if no element in the subtree is greater than 
 * 				@a val.
 */
template <class MultiwayCursor, class T>
MultiwayCursor upper_bound(MultiwayCursor x, T const& val)
{
	MultiwayCursor y = x;
	while (!x.empty()) {
		x = std::upper_bound(x.begin(), x.end(), val);
		if (x.parity() == 0)
			y = x;
	}
	return y;
}

/**
 * @brief		Finds the last position in a multiway subtree in which @a val 
 * 				could be inserted without changing the ordering, using @a cmp 
 * 				for comparisons.
 * @param x		The subtree's root
 * @param val	The search term
 * @param cmp	The comparison functor
 * @return		A multiway cursor pointing to the first element greater than 
 *				@a val, or @x if no element in the subtree is greater than
 * 				@a val.
 */
template <class MultiwayCursor, class T, class Cmp>
MultiwayCursor upper_bound(MultiwayCursor x, T const& val, Cmp cmp)
{
	MultiwayCursor y = x;
	while (!x.empty()) {
		x = std::upper_bound(x.begin(), x.end(), val, cmp);
		if (x.parity() == 0)
			y = x;
	}
	return y;
}

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_CURSOR_INORDER_HPP
