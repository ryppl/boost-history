//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file    _order.hpp
 * 
 * Some definitions that are identical for all *order cursors (as we are just
 * calling the appropriate traversal function that are defined in the 
 * respective *order.hpp files).
 */

// NO guards, as this is context-included!

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

// TODO: concept checks.

/**
 * @brief    First element of a subtree in traversal
 *             (equivalent to postorder::begin())
 * @param c    A cursor
 * @return    Iterator to the first element of @a c
 */
template <class Cursor>
iterator<Cursor> 
begin(Cursor c)
{
    return iterator<Cursor>(first(c));
}

/**
 * @brief    One position past the last element of a subtree 
 *             in traversal (Alias of cend())
 * @param c    A cursor
 * @return    Iterator one position past the last element of @a c 
 */
template <class Cursor>
iterator<Cursor>
end(Cursor c)
{
    return iterator<Cursor>(last(c));
}

/// Reverse iterators

template <class Cursor>
std::reverse_iterator< iterator<Cursor> >
rbegin(Cursor c)
{
    return std::reverse_iterator< iterator<Cursor> >(end(c));
}

template <class Cursor>
std::reverse_iterator< iterator<Cursor> >
rend(Cursor c)
{
    return std::reverse_iterator< iterator<Cursor> >(begin(c));
}
