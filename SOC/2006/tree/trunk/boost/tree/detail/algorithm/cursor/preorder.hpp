//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder.hpp
 * Subtree preorder traversal algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_PREORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_PREORDER_HPP

#include <boost/tree/root_tracking_cursor.hpp>
#include <boost/tree/ascending_cursor.hpp>

struct preorder {};

namespace boost {
namespace tree {

/** \addtogroup traversal */
/*\@{*/

/**
 * @brief    Preorder successor
 * @param c    Cursor to be set to its preorder successor
 */
template <class Cursor>
inline void forward<preorder>(Cursor& c)
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
        if (!c.is_root() && !c.parity()) {
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
inline void back<preorder>(Cursor& c)
{
    if (!c.is_root()) {
        c.to_parent();
        
        // If a left child was given, just move to its parent.
        if (!c.parity())
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

    if (c.parity())
        --c;
    return;
}

/**
 * @brief   First element of a subtree in preorder traversal
 * @param c Subtree root cursor that will be set to the first preorder 
 *          position in the subtree.
 */
template <class Cursor>
void to_first<preorder>(Cursor& c)
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
void to_last<preorder>(Cursor& c)
{ }

/*\@}*/

#ifndef BOOST_RECURSIVE_ORDER_ALGORITHMS
#include <boost/tree/detail/algorithm/cursor/_order_iterative.hpp>
#else

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class Cursor, class Op>
void for_each_recursive<preorder>(Cursor s, Op& f)
{
    Cursor t = s.end();
    for (s.to_begin(); s != t; ++s) {
        f(*s);
        if (!s.empty())
            for_each_recursive<preorder>(s, f);
    }
    
    // Multiway cursor
    if (!s.empty())
        for_each_recursive<preorder>(s, f);
}

/**
 * @brief    Apply a function to every element of a subtree, in preorder.
 * @param s    A cursor.
 * @param f    A unary function object.
 * @return    @p f
 *
 * Applies the function object @p f to each element in the @p subtree, using  
 * preorder. @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
//[ preorder_for_each
template <class Cursor, class Op>
Op for_each<preorder>(Cursor s, Op f)
//]
{
    Cursor t = s.end();
    for (s.to_begin(); s != t; ++s) {
        f(*s);
        if (!s.empty())
            for_each_recursive<preorder>(s, f);
    }
    
    // Multiway cursor
    if (!s.empty())
        for_each_recursive<preorder>(s, f);
    
    return f;
}

//#endif //BOOST_RECURSIVE_ORDER_ALGORITHMS

/**
 * @brief    Copies the subtree s into t, by traversing s in preorder.
 * @param s    An input cursor.
 * @param t An output cursor.
 * @result    A cursor past t's preorder end, after the copying operation.
 */
template <class InCursor, class OutCursor>
OutCursor copy<preorder>(InCursor s, OutCursor t)
{
    InCursor r = s.end();
    s.to_begin();
    t.to_begin();
    
    for (; s != r; ++s, ++t) {
        *t = *s;
        if (!s.empty())
            copy<preorder>(s, t);
    }

    // Multiway cursor
    if (!r.empty())
        copy<preorder>(r, t);

    return t;
}

/**
 * @brief     Performs an operation on a subtree, by traversing it in preorder.
 * @param s  An input cursor.
 * @param t  An output cursor.
 * @param op A unary operation.
 * @result     A cursor past t's preorder end, after the transforming has 
 *              finished.
 * 
 * By traversing the input subtree s in preorder, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class InCursor, class OutCursor, class Op>
OutCursor transform<preorder>(InCursor s, OutCursor t, Op op)
{
    InCursor r = s.end();
    s.to_begin();
    t.to_begin();
    for (; s != r; ++s, ++t) {
        *t = op(*s);
        if (!s.empty())
            transform<preorder>(s, t, op);
    }

    // Multiway cursor
    if (!s.empty())
        transform<preorder>(s, t, op);
        
    return t;
}

#endif //BOOST_RECURSIVE_ORDER_ALGORITHMS

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_CURSOR_PREORDER_HPP
