//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder_algorithms.hpp
 * Subtree preorder traversal algorithms
 */

#ifndef BOOST_TREE_PREORDER_ALGORITHMS_HPP
#define BOOST_TREE_PREORDER_ALGORITHMS_HPP

#include <boost/tree/root_tracking_cursor.hpp>
#include <boost/tree/ascending_cursor.hpp>
#include <boost/tree/cursor_concepts.hpp>

#include <boost/concept/requires.hpp>

namespace boost {
namespace tree {

using namespace boost_concepts;

/** \addtogroup traversal */
/*\@{*/

struct preorder {
    typedef bidirectional_traversal_tag iterator_category;
};

/**
 * @brief    Preorder successor
 * @param c    Cursor to be set to its preorder successor
 */
template <typename Cursor>
inline
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>))
    ((RootTracker<Cursor>)),
    (void)) // return type
successor(preorder, Cursor& c)
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
    while (!c.is_root()) { // Doesn't work with subtrees!    
        c.to_parent();
        if (!c.is_root() && !index(c)) {
            if (!(++c).empty()) {
                c.to_begin();
                return;
            }
        }
    }
    return;
}

/**
 * @brief    Preorder predecessor
 * @param c    Cursor to be set to its preorder predecessor
 */
template <class Cursor>
inline
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>))
    ((RootTracker<Cursor>)),
    (void)) // return type
predecessor(preorder, Cursor& c)
{
    if (!c.is_root()) {
        c.to_parent();
        
        // If a left child was given, just move to its parent.
        if (!index(c))
            return;
        
        // So this is a right child.
        --c;
    }
    
    // Same for root (=end) and right children:
    while (!c.empty())
        if (!c.end().empty())
            c.to_end();
        else
            c.to_begin();

    if (index(c))
        --c;
    return;
}

/**
 * @brief   First element of a subtree in preorder traversal
 * @param c Subtree root cursor that will be set to the first preorder 
 *          position in the subtree.
 */
template <class Cursor>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>)),
    (void)) // return type
to_first(preorder, Cursor& c)
{
    c.to_begin();
}

/**
 * @brief   One position past the last element of a subtree in preorder 
 *          traversal
 * @param c Subtree root cursor that will be set to the last preorder 
 *          position in the subtree.
 */
template <class Cursor>
void to_last(preorder, Cursor& c)
{ }

/*\@}*/

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_PREORDER_ALGORITHMS_HPP
