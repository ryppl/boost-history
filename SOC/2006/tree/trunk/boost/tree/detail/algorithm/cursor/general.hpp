//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file general.hpp
 * General algorithms for cursors
 */


#ifndef BOOST_TREE_DETAIL_ALGORITHM_GENERAL_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_GENERAL_HPP


namespace boost {
namespace tree {

// These algorithms are actually mostly preorder, as it's most efficient, but I
// think it doesn't make much sense having in- and postorder versions of them. 
// The only reason I can think of is that there might be some cases
// where it's likely to find a mismatch or whatever faster in in- or postorder
// than in preorder -- but for things like size() or count(), this doesn't really matter.

// What about the subtree shapes?
/**
 *  @brief        Checks two subtrees for element-wise equality.
 *  @param c1    An input cursor.
 *  @param c2    An input cursor.
 *  @return        A boolean true or false.
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

/**
 *  @brief        Checks two subtrees for element-wise equality.
 *  @param c1    An input cursor.
 *  @param c2    An input cursor.
 *  @param p    A binary predicate.
 *  @return        A boolean true or false.
 *
 *  Compares the elements of two subtrees using the p parameter. 
 *  Returns true if all the corresponding elements of the 
 *  subtrees are equal; otherwise, it returns false.
 */
template <class InCursor1, class InCursor2, class BinPred>
bool equal(InCursor1 c1, InCursor2 c2, BinPred p)
{
    InCursor1 d1 = c1.end();
    c1.to_begin();
    c2.to_begin();
    if (!p(*c1,*c2))
        return false;
    do {
        if (!c1.empty())
            if (!equal(c1, c2))
                return false;
        ++c2;
    } while (c1++ != d1);
    
    return true;
}

/**
 *  @brief        Calculates the number of elements in a subtree.
 *  @param c    An input cursor.
 *  @param s    The size type of @c c1.
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
 *  @brief        Returns the number of elements in a subtree.
 *  @param c    An input cursor.
 *  @return        The size type of @c c1.
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

#endif // BOOST_TREE_DETAIL_ALGORITHM_GENERAL_HPP
