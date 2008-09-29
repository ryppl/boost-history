//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file    cursor.hpp
 * 
 * Some algorithm versions that are identical for all *order cursors 
 * (as we are just calling the appropriate traversal function that are 
 * defined in the respective *order.hpp files).
 */

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_HPP

#include <boost/tree/detail/algorithm/cursor/preorder.hpp>
#include <boost/tree/detail/algorithm/cursor/inorder.hpp>
#include <boost/tree/detail/algorithm/cursor/postorder.hpp>

#ifndef BOOST_RECURSIVE_ORDER_ALGORITHMS
#include <boost/tree/detail/algorithm/cursor/iterative.hpp>
#endif

namespace boost {
namespace tree {

/**
 * @brief   Successor
 * @param c A cursor
 * @param n Optional parameter to indicate how many steps to move forward
 * @return  Successor of @a c
 */
template <class Order, class Cursor>
inline Cursor next(Cursor c
                 , typename Cursor::difference_type n = 1)
{
    for (; n!=0; --n)
        forward<Order>(c);
    return c;
}

/**
 * @brief   Predecessor
 * @param c A cursor
 * @param n Optional parameter to indicate how many steps to move back
 * @return  Predecessor of @a c
 */
template <class Order, class Cursor>
inline Cursor prior(Cursor c
                  , typename Cursor::difference_type n = 1)
{
    for (; n!=0; --n)
        back<Order>(c);
    return c;
}

/**
 * @brief   First element of a subtree in preorder traversal
 * @param c A cursor
 * @return  Cursor to the first element of @a c in preorder traversal
 */
template <class Order, class Cursor>
Cursor first(Cursor c)
{
    to_first<Order>(c);
    return c;
}

/**
 * @brief   One position past the last element of a subtree in preorder 
 *          traversal
 * @param c A cursor
 * @return  Cursor one position past the last element of @a c in preorder
 *          traversal
 */
template <class Order, class Cursor>
Cursor last(Cursor c)
{
    to_last<Order>(c);
    return c;
}

} // namespace tree
} // namespace boost

#endif //BOOST_TREE_DETAIL_ALGORITHM_CURSOR_HPP