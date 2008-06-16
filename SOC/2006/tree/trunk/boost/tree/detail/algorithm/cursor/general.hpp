//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file general.hpp
 * General algorithms for cursors
 */
//TODO:
// Concept checks

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_GENERAL_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_GENERAL_HPP


namespace boost {
namespace tree {


// What about the subtree shapes?
/**
 *  @brief		Checks two subtrees for element-wise equality.
 *  @param c1	An input cursor.
 *  @param c2	An input cursor.
 *  @return		A boolean true or false.
 *
 *  Compares the elements of two subtrees using @c ==. Returns true if
 *  all the corresponding elements of the subtrees are equal; otherwise,
 *  it returns false.
 */
template <class InCursor1, class InCursor2>
bool equal(InCursor1 c1, InCursor2 c2)
{
	InCursor1 d1 = c1.end();
	c1.to_begin();
	c2.to_begin();
	if (!(*c1 == *c2))
		return false;
	do {
		if (!c1.empty())
			if (!equal(c1, c2))
				return false;
		++c2;
	} while (c1++ != d1);
	
	return true;
}


// Is it really a good idea to use InCursor::size_type?
// For a binary_cursor, a boolean size_type would be enough - but
// not for a subtree algorithm like this one.
/**
 *  @brief		Calculates the number of elements in a subtree.
 *  @param c	An input cursor.
 *  @param s	The size type of @c c1.
 * 
 * After finishing, s will have been increased by the number of elements in c. 		
 */
template <class InCursor>
void size(InCursor c, typename InCursor::size_type& s)
{
	InCursor d = c.end();
	c.to_begin();
	++s;
	do
		if (!c.empty())
			size(c, s);
	while (c++ != d);
}


/**
 *  @brief		Returns the number of elements in a subtree.
 *  @param c	An input cursor.
 *  @return		The size type of @c c1.
 */
template <class InCursor>
typename InCursor::size_type size(InCursor c)
{
	typename InCursor::size_type s = 0;
	InCursor d = c.end();
	c.to_begin();
	++s;
	do
		if (!c.empty())
			size(c, s);
	while (c++ != d);
	
	return s;
}


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_CURSOR_GENERAL_HPP
