//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file    iterator.hpp
 * 
 * Some definitions that are identical for all *order cursors (as we are just
 * calling the appropriate traversal function that are defined in the 
 * respective *order.hpp files).
 */

#ifndef BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_HPP

#include <boost/tree/iterator.hpp>
#include <boost/tree/detail/algorithm/cursor.hpp>

namespace boost {
namespace tree {

/**
 * @brief    First element of a subtree in traversal
 *             (equivalent to postorder::begin())
 * @param c    A cursor
 * @return    Iterator to the first element of @a c
 */
template <class Order, class Cursor>
iterator<Order, Cursor>
begin(Order, Cursor c)
{
    to_first(Order(), c);
    return iterator<Order, Cursor>(c);
}

/**
 * @brief    One position past the last element of a subtree 
 *             in traversal (Alias of cend())
 * @param c    A cursor
 * @return    Iterator one position past the last element of @a c 
 */
template <class Order, class Cursor>
iterator<Order, Cursor>
end(Order, Cursor c)
{
    to_last(Order(), c);
    return iterator<Order, Cursor>(c);
}

/// Reverse iterators

template <class Order, class Cursor>
std::reverse_iterator< iterator<Order, Cursor> >
rbegin(Order, Cursor c)
{
    return std::reverse_iterator< iterator<Order, Cursor> >(end(Order(), c));
}

template <class Order, class Cursor>
std::reverse_iterator< iterator<Order, Cursor> >
rend(Order, Cursor c)
{
    return std::reverse_iterator< iterator<Order, Cursor> >(begin(Order(), c));
}

} // namespace tree
} // namespace boost

#endif //BOOST_TREE_DETAIL_ALGORITHM_ITERATOR_HPP