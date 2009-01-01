//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file   algorithm.hpp
 * 
 * Hierarchy algorithms
 */

#ifndef BOOST_TREE_ALGORITHM_HPP
#define BOOST_TREE_ALGORITHM_HPP

#include <boost/tree/detail/algorithm/general.hpp>
#include <boost/tree/detail/algorithm/ascending.hpp>

#include <boost/tree/detail/algorithm/preorder.hpp>
#include <boost/tree/detail/algorithm/inorder.hpp>
#include <boost/tree/detail/algorithm/postorder.hpp>

//#ifndef BOOST_RECURSIVE_ORDER_ALGORITHMS
#include <boost/tree/detail/algorithm/iterative.hpp>
//#endif

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
        successor<Order>(c);
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
        predecessor<Order>(c);
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

template <class Cursor>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<Cursor>))
    ((Ascendor<Cursor>)),
    (typename Cursor::size_type)) // return type
index(Cursor const& cur)
{
    return std::distance(cur.parent().begin(), cur);
}

template <class BinaryCursor>
void to_forest_end(BinaryCursor& c)
{
    c.to_begin();
    while (!c.empty())
        c.to_end();
}


template <class BinaryCursor>
BOOST_CONCEPT_REQUIRES(
    ((Descendor<BinaryCursor>))
    ((Ascendor<BinaryCursor>)),
    (void)) // return type
to_forest_parent(BinaryCursor& c)
{
    while (index(c))
        c.to_parent();
    c.to_parent();
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
    return for_each(Order(), s, f
                  , typename cursor_vertical_traversal<Cursor>::type());
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
//[ transform
template <class Order, class InCursor, class OutCursor, class Op>
OutCursor transform (Order, InCursor s, OutCursor t, Op op)
//]
{
    return transform(Order(), s, t, op
                  , typename cursor_vertical_traversal<InCursor>::type());
    // What about OutCursor?
}

template <class T>
T identity (T const& x)
{
    return x;
}

template <class Order, class InCursor, class OutCursor, class Traversal>
OutCursor copy(Order, InCursor s, OutCursor t, Traversal tr)
{
    return transform(Order(), s, t
                   , identity<typename cursor_value<InCursor>::type>, tr);
}

/**
 * @brief   Copies the subtree s into t, by traversing s
 *          in the order specified by the first parameter.
 * @param s An input cursor.
 * @param t An output cursor.
 * @result  A cursor past t's *order end, after the copying operation.
 */
//[ copy
template <class Order, class InCursor, class OutCursor>
OutCursor copy (Order, InCursor s, OutCursor t)
//]
{
    return copy(Order(), s, t
              , typename cursor_vertical_traversal<InCursor>::type());
    // What about OutCursor?
}

} // namespace tree
} // namespace boost

#endif //BOOST_TREE_ALGORITHM_HPP