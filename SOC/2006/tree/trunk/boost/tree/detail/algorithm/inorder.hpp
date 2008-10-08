//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder.hpp
 * Subtree intorder traversal and search algorithms
 */

#ifndef BOOST_TREE_DETAIL_ALGORITHM_INORDER_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_INORDER_HPP

#include <boost/tree/root_tracking_cursor.hpp>
#include <boost/tree/ascending_cursor.hpp>

#include <algorithm>

namespace boost {
namespace tree {

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
inline void forward(inorder, MultiwayCursor& c)
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
 * @brief    Inorder predecessor
 * @param c    MultiwayCursor to be set to its inorder predecessor
 */
template <class MultiwayCursor>
inline void back(inorder, MultiwayCursor& c)
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
 * @brief   First element of a subtree in inorder traversal
 * @param c Subtree root cursor that will be set to the first inorder 
 *          position in the subtree.
 */
template <class Cursor>
void to_first(inorder, Cursor& c)
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

#ifdef BOOST_RECURSIVE_ORDER_ALGORITHMS

/**
 * @if maint
 * Helper function for for_each, using a reference parameter in order to
 * require fewer copy and assignment operations.
 * @endif
 */
template <class MultiwayCursor, class Op>
void for_each_recursive(inorder, MultiwayCursor s, Op& f)
{
    MultiwayCursor t = s.end();

    for (s.to_begin(); s!=t; ++s) {
        if (!s.empty())
            for_each_recursive(inorder(), s, f);
        f(*s);
    }
    
    // Multiway cursor
    if (!t.empty())
        for_each_recursive(inorder(), t, f);
}

/**
 * @brief    Apply a function to every element of a multiway subtree,
 *            in inorder.
 * @param s    A MultiwayTree cursor.
 * @param f    A unary function object.
 * @return    @p f
 *
 * Applies the function object @p f to each element in the @p subtree, using
 * inorder. @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
 //[ inorder_for_each
template <class MultiwayCursor, class Op>
Op for_each(inorder, MultiwayCursor s, Op f)
//]
{
    MultiwayCursor t = s.end();

    for (s.to_begin(); s!=t; ++s) {
        if (!s.empty())
            for_each_recursive(inorder(), s, f);
        f(*s);
    }
    
    // Multiway cursor
    if (!t.empty())
        for_each_recursive(inorder(), t, f);
    return f;
}

/**
 * @brief    Copies the subtree s into t, by traversing s in inorder.
 * @param s    An input cursor.
 * @param t An output cursor.
 * @result    A cursor past t's inorder end, after the copying operation.
 */
template <class InCursor, class OutCursor>
OutCursor copy(inorder, InCursor s, OutCursor t)
{
    InCursor r = s.end();

    s.to_begin();
    t.to_begin();
    
    for (; s != r; ++s, ++t) {
        if (!s.empty())
            copy(inorder(), s, t);
        *t=*s;
    }
    
    // Multiway cursor
    if (!r.empty())
        copy(inorder(), r, t);
    return t;
}

/**
 * @brief     Performs an operation on a subtree, by traversing it in inorder.
 * @param s  An input cursor.
 * @param t  An output cursor.
 * @param op A unary operation.
 * @result     A cursor past t's inorder end, after the transforming has 
 *              finished.
 * 
 * By traversing the input subtree s in inorder, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class InCursor, class OutCursor, class Op>
OutCursor transform(inorder, InCursor s, OutCursor t, Op op)
{
    InCursor r = s.end();

    s.to_begin();
    t.to_begin();
    
    for (; s != r; ++s, ++t) {
        if (!s.empty())
            transform(inorder(), s, t, op);
        *t=op(*s);
    }

    // Multiway cursor
    if (!r.empty())
        transform(inorder(), r, t, op);
    return t;
}

#endif //BOOST_RECURSIVE_ORDER_ALGORITHMS

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
 * @brief        Finds the last position in a multiway subtree in which @a val 
 *                 could be inserted without changing the ordering, using < (less
 *                 than) for comparisons.
 * @param x        The subtree's root
 * @param val    The search term
 * @return        A multiway cursor pointing to the first element greater than 
 *                @a val, or @x if no element in the subtree is greater than 
 *                 @a val.
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

// Implement equal_range? Maybe not, if it'd only return a pair 
// consisting of cursors calculated by calling lower_bound and upper_bound.
// This might be a bit more subtle for non-binary multiway trees. 

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_INORDER_HPP
