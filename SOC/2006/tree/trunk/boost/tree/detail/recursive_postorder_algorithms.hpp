//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file postorder.hpp
 * Subtree postorder traversal algorithms
 */

#ifndef BOOST_TREE_DETAIL_ALGORITHM_POSTORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_POSTORDER_HPP

#include <boost/tree/root_tracking_cursor.hpp>
#include <boost/tree/ascending_cursor.hpp>
#include <boost/tree/cursor_concepts.hpp>

#include <boost/concept/requires.hpp>

namespace boost {
namespace tree {

using namespace boost_concepts;

/** \addtogroup traversal */
/*\@{*/

struct postorder {
    typedef bidirectional_traversal_tag iterator_category;
};

/**
 * @brief    Postorder successor
 * @param c    Cursor to be set to its postorder successor
 */
template <class Cursor>
inline
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>))
    ((RootTracker<Cursor>)),
    (void)) // return type
successor(postorder, Cursor& c)
{
    c.to_parent();

    if (c.is_root())
        return;

    if (index(c)) { // Right child? Return parent.
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
    if (index(c))
        --c;
    return;
}

/**
 * @brief    Postorder predecessor
 * @param c    Cursor to be set to its postorder predecessor
 */
template <class Cursor>
inline
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>))
    ((RootTracker<Cursor>)),
    (void)) // return type
predecessor(postorder, Cursor& c)
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
    while (!c.is_root()) {
        c.to_parent();
        if (index(c))
            if (!(--c).empty()) {
                c.to_begin();
                return;
            }
    }
    return;
}

/**
 * @brief   First element of a subtree in postorder traversal
 * @param c Subtree root cursor that will be set to the first postorder 
 *          position in the subtree.
 */
template <class Cursor>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>)),
    (void)) // return type
to_first(postorder, Cursor& c)
{
    while (true)
        if (!c.empty())
            c.to_begin();
        else if (!(++c).empty())
            c.to_begin();
        else {
            --c;
            return;
        }
}

/**
 * @brief   One position past the last element of a subtree in postorder 
 *          traversal
 * @param c Subtree root cursor that will be set to the last postorder 
 *          position in the subtree.
 */
template <class Cursor>
void to_last(postorder, Cursor& c)
{ }

/*\@}*/

//#ifdef BOOST_RECURSIVE_ORDER_ALGORITHMS

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class Cursor, class Op>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>)),
    (void)) // return type
for_each_recursive(postorder, Cursor s, Op& f)
{
    Cursor t = s;
    for (s.to_begin(); s != t.end(); ++s)
        if (!s.empty())
            for_each_recursive(postorder(), s, f);

    // Multiway cursor
    if (!s.empty())
        for_each_recursive(postorder(), s, f);

    f(*t.to_begin());
}

/**
 * @brief    Apply a function to every element of a subtree, in postorder.
 * @param s    A cursor.
 * @param f    A unary function object.
 * @return    @p f
 *
 * Applies the function object @p f to each element in the subtree @p s, using 
 * postorder. @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
template <class Cursor, class Op>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>)),
    (Op)) // return type
for_each(postorder, Cursor s, Op f, descending_vertical_traversal_tag)
{
    Cursor t = s;
    for (s.to_begin(); s != t.end(); ++s)
        if (!s.empty())
            for_each_recursive(postorder(), s, f);

    // Multiway cursor
    if (!s.empty())
        for_each_recursive(postorder(), s, f);

    f(*t.to_begin());

    return f;
}

/**
 * @brief     Performs an operation on a subtree, by traversing it in postorder.
 * @param s  An input cursor.
 * @param t  An output cursor.
 * @param op A unary operation.
 * @result     A cursor past t's postorder end, after the transforming has
 *              finished.
 * 
 * By traversing the input subtree s in postorder, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class InCursor, class OutCursor, class Op>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<InCursor>))
    ((Descendor<OutCursor>)),
    (OutCursor)) // return type
transform(postorder, InCursor s, OutCursor t, Op op, descending_vertical_traversal_tag)
{
    InCursor r = s;
    s.to_begin();
    t.to_begin();
    OutCursor t2 = t;
    
    for (; s != r.end(); ++s, ++t)
        if (!s.empty())
            transform(postorder(), s, t, op, descending_vertical_traversal_tag());

    // Multiway cursor
    if (!s.empty())
        transform(postorder(), s, t, op, descending_vertical_traversal_tag());
    
    *t2 = op(*r.to_begin());
    return t;
}

//#endif //BOOST_RECURSIVE_ORDER_ALGORITHMS

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_POSTORDER_HPP
