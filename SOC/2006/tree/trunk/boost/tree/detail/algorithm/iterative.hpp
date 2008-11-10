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

#include <boost/tree/coupling_cursor.hpp>

#include <boost/tree/detail/algorithm/preorder.hpp>
#include <boost/tree/detail/algorithm/inorder.hpp>
#include <boost/tree/detail/algorithm/postorder.hpp>

namespace boost {
namespace tree {

template <class Order, class Cursor, class Op>
Op for_each(Order, Cursor is, Op f, bidirectional_traversal_tag)
{
    root_tracking_cursor<Cursor> s(is);
    root_tracking_cursor<Cursor> s2(s);
    to_first(Order(), s);
    to_last(Order(), s2);
    while (s!=s2) {
        f(*s);
        forward(Order(), s);
    }
    return f;
}

template <class Order, class InCursor, class OutCursor, class Op>
OutCursor transform (Order, InCursor is, OutCursor t, Op op
                   , bidirectional_traversal_tag)
{
    root_tracking_cursor<InCursor> s(is);
    root_tracking_cursor<InCursor> s2(s);
    
    boost::tree::coupling_cursor< root_tracking_cursor<InCursor>, OutCursor > cc(s, t);

    to_first(Order(), cc);
    to_last(Order(), s2);

    while (cc.in()!=s2) {
        *cc.out() = op(*cc.in());
        forward(Order(), cc);
    }
    return cc.out();
}

} // namespace tree
} // namespace boost

#endif //BOOST_TREE_DETAIL_ALGORITHM_ITERATIVE_HPP