//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder_algorithms.hpp
 * Subtree intorder traversal and search algorithms
 */

#ifndef BOOST_TREE_INORDER_ALGORITHMS_HPP
#define BOOST_TREE_INORDER_ALGORITHMS_HPP

#include <boost/tree/root_tracking_cursor.hpp>
#include <boost/tree/ascending_cursor.hpp>
#include <boost/tree/cursor_concepts.hpp>

#include <boost/concept/requires.hpp>

#include <algorithm>

namespace boost {
namespace tree {

using namespace boost_concepts;

/** \addtogroup traversal */
/*\@{*/

struct inorder {
    typedef bidirectional_traversal_tag iterator_category;
};

/**
 * @brief    Inorder successor
 * @param c    MultiwayCursor to be set to its inorder successor
 * @ingroup    traversal
 */
template <class MultiwayCursor>
inline
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<MultiwayCursor>))
    ((RootTrackingCursor<MultiwayCursor>)),
    (void)) // return type
successor(inorder, MultiwayCursor& c)
{
    if (!(++c).empty()) {
        while (!c.to_begin().empty());
        return;
    }
    
    while (index(c) && !c.is_root())
        c.to_parent();
    return;
}

/**
 * @brief    Inorder predecessor
 * @param c    MultiwayCursor to be set to its inorder predecessor
 */
template <class MultiwayCursor>
inline
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<MultiwayCursor>))
    ((RootTrackingCursor<MultiwayCursor>)),
    (void)) // return type
predecessor(inorder, MultiwayCursor& c)
{
    if (!c.empty()) {
        while (!c.to_end().empty());
        --c;
        return;
    }
    
    while (!index(c) && !c.is_root())
        c.to_parent();
    if (!c.is_root())
        --c;
    return;
}

/**
 * @brief   First element of a subtree in inorder traversal
 * @param c Subtree root cursor that will be set to the first inorder 
 *          position in the subtree.
 */
template <class Cursor>
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<Cursor>)),
    (void)) // return type
to_first(inorder, Cursor& c)
{
    while (!c.empty())
        c.to_begin();
}

/**
 * @brief   One position past the last element of a subtree in inorder 
 *          traversal
 * @param c Subtree root cursor that will be set to the last inorder 
 *          position in the subtree.
 */
template <class Cursor>
void to_last(inorder, Cursor& c)
{ }

/*\@}*/

/// Search

/**
 * @brief        Finds the first position in a multiway subtree in which @a val 
 *                 could be inserted without changing the ordering, using < (less
 *                 than) for comparisons.
 * @param x        The subtree's root
 * @param val    The search term
 * @return        A multiway cursor pointing to the first element not less than 
 *                @a val, or @x if every element in the subtree is less than 
 *                 @a val.
 */
//[ lower_bound
template <class MultiwayCursor, class T>
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<MultiwayCursor>)),
    (MultiwayCursor)) // return type
lower_bound(MultiwayCursor x, T const& val)
//]
{
    MultiwayCursor y = x;
    while (!x.empty()) {
        x = std::lower_bound(x.begin(), x.end(), val);
        if (index(x) == 0)
            y = x;
    }
    return y;
}

/**
 * @brief        Finds the first position in a multiway subtree in which @a val 
 *                 could be inserted without changing the ordering, using @a cmp 
 *                 for comparisons.
 * @param x        The subtree's root
 * @param val    The search term
 * @param cmp    The comparison functor
 * @return        A multiway cursor pointing to the first element not less than 
 *                @a val, or @x if every element in the subtree is less than 
 *                 @a val.
 */
//[ lower_bound_cmp
template <class MultiwayCursor, class T, class Cmp>
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<MultiwayCursor>))
    /*((LessThanComparable<Cmp>))*/, // Problem with balance
    (MultiwayCursor)) // return type
lower_bound(MultiwayCursor x, T const& val, Cmp cmp)
//]
{
    MultiwayCursor y = x;
    while (!x.empty()) {
        x = std::lower_bound(x.begin(), x.end(), val, cmp);
        if (index(x) == 0)
            y = x;
    }
    return y;
}

/**
 * @brief        Finds the last position in a multiway subtree in which @a val 
 *                 could be inserted without changing the ordering, using < (less
 *                 than) for comparisons.
 * @param x        The subtree's root
 * @param val    The search term
 * @return        A multiway cursor pointing to the first element greater than 
 *                @a val, or @x if no element in the subtree is greater than 
 *                 @a val.
 */
//[ upper_bound
template <class MultiwayCursor, class T>
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<MultiwayCursor>)),
    (MultiwayCursor)) // return type
upper_bound(MultiwayCursor x, T const& val)
//]
{
    MultiwayCursor y = x;
    while (!x.empty()) {
        x = std::upper_bound(x.begin(), x.end(), val);
        if (index(x) == 0)
            y = x;
    }
    return y;
}

/**
 * @brief        Finds the last position in a multiway subtree in which @a val 
 *                 could be inserted without changing the ordering, using @a cmp 
 *                 for comparisons.
 * @param x        The subtree's root
 * @param val    The search term
 * @param cmp    The comparison functor
 * @return        A multiway cursor pointing to the first element greater than 
 *                @a val, or @x if no element in the subtree is greater than
 *                 @a val.
 */
//[ upper_bound_cmp
template <class MultiwayCursor, class T, class Cmp>
BOOST_CONCEPT_REQUIRES(
    ((DescendingCursor<MultiwayCursor>))
    ((LessThanComparable<Cmp>)),
    (MultiwayCursor)) // return type
upper_bound(MultiwayCursor x, T const& val, Cmp cmp)
//]
{
    MultiwayCursor y = x;
    while (!x.empty()) {
        x = std::upper_bound(x.begin(), x.end(), val, cmp);
        if (index(x) == 0)
            y = x;
    }
    return y;
}

// Implement equal_range? Maybe not, if it'd only return a pair 
// consisting of cursors calculated by calling lower_bound and upper_bound.
// This might be a bit more subtle for non-binary multiway trees. 

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_ALGORITHMS_HPP
