//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file   iterative.hpp
 * 
 * Some iterative algorithm versions that are identical for all *order cursors 
 * (as we are just calling the appropriate traversal function that are 
 * defined in the respective *order.hpp files).
 */

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_ITERATIVE_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_ITERATIVE_HPP

#include <boost/tree/detail/algorithm/cursor/preorder.hpp>
#include <boost/tree/detail/algorithm/cursor/inorder.hpp>
#include <boost/tree/detail/algorithm/cursor/postorder.hpp>

namespace boost {
namespace tree {

template <class Order, class Cursor, class Op>
Op for_each(Order, root_tracking_cursor<Cursor> s, Op f)
{
    root_tracking_cursor<Cursor> s2(s);
    to_first(Order(), s);
    to_last(Order(), s2);
    while (s!=s2) {
        f(*s);
        forward(Order(), s);
    }
    return f;
}

/**
 * @brief   Apply a function to every element of a subtree, 
 *          in the order specified by the first parameter.
 * @param s A cursor.
 * @param f A unary function object.
 * @return  @p f
 *
 * Applies the function object @p f to each element in the @p subtree. 
 * @p f must not modify the order of the sequence.
 * If @p f has a return value it is ignored.
 */
//[ for_each
template <class Order, class Cursor, class Op>
Op for_each(Order, Cursor s, Op f)
//]
{
    return for_each(Order(), root_tracking_cursor<Cursor>(s), f);
}

template <class Order, class InCursor, class OutCursor>
root_tracking_cursor<OutCursor> copy (Order, root_tracking_cursor<InCursor> s
                                    , root_tracking_cursor<OutCursor> t)
{
    root_tracking_cursor<InCursor> s2(s);
    to_first(Order(), s);
    to_last(Order(), s2);
    to_first(Order(), t);
    while (s!=s2) {
        *t = *s;
        forward(Order(), s);
        forward(Order(), t);
    }
    return t;
}

/**
 * @brief   Copies the subtree s into t, by traversing s
 *          in the order specified by the first parameter.
 * @param s An input cursor.
 * @param t An output cursor.
 * @result  A cursor past t's *order end, after the copying operation.
 */
template <class Order, class InCursor, class OutCursor>
OutCursor copy (Order, InCursor s, OutCursor t)
{
    root_tracking_cursor<OutCursor> u 
        = copy(Order(), root_tracking_cursor<InCursor>(s)
                    , root_tracking_cursor<OutCursor>(t));
    return u.base();
}

template <class Order, class InCursor, class OutCursor, class Op>
root_tracking_cursor<OutCursor> transform (Order
                                         , root_tracking_cursor<InCursor> s
                                         , root_tracking_cursor<OutCursor> t
                                         , Op op)
{
    root_tracking_cursor<InCursor> s2(s);
    to_first(Order(), s);
    to_last(Order(), s2);
    to_first(Order(), t);
    while (s!=s2) {
        *t = op(*s);
        forward(Order(), s);
        forward(Order(), t);
    }
    return t;
}

/**
 * @brief       Performs an operation on a subtree, by traversing it  
 *              in the order specified by the first parameter.
 * @param s     An input cursor.
 * @param t     An output cursor.
 * @param op    A unary operation.
 * @result      A cursor past t's preorder end, after the transforming has 
 *              finished.
 * 
 * By traversing the input subtree s, apply the operation op 
 * to each element and write the result to the output subtree t.
 * 
 * op must not change its argument.
 */
template <class Order, class InCursor, class OutCursor, class Op>
OutCursor transform (Order, InCursor s, OutCursor t, Op op)
{
    root_tracking_cursor<OutCursor> u 
        = transform(Order(), root_tracking_cursor<InCursor>(s)
                  , root_tracking_cursor<OutCursor>(t), op);
    return u.base();
}

} // namespace tree
} // namespace boost

#endif //BOOST_TREE_DETAIL_ALGORITHM_CURSOR_ITERATIVE_HPP