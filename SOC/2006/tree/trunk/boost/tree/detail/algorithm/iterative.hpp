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

#ifndef BOOST_TREE_DETAIL_ALGORITHM_ITERATIVE_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_ITERATIVE_HPP

#include <boost/tree/detail/algorithm/preorder.hpp>
#include <boost/tree/detail/algorithm/inorder.hpp>
#include <boost/tree/detail/algorithm/postorder.hpp>

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

template <class Order, class Cursor, class Op>
Op for_each(Order, Cursor s, Op f, bidirectional_traversal_tag)
{
    return for_each(Order(), root_tracking_cursor<Cursor>(s), f);
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

template <class Order, class InCursor, class OutCursor, class Op>
OutCursor transform (Order, InCursor s, OutCursor t, Op op
                   , bidirectional_traversal_tag)
{
    root_tracking_cursor<OutCursor> u 
        = transform(Order(), root_tracking_cursor<InCursor>(s)
                  , root_tracking_cursor<OutCursor>(t), op);
    return u.base();
}



} // namespace tree
} // namespace boost

#endif //BOOST_TREE_DETAIL_ALGORITHM_ITERATIVE_HPP