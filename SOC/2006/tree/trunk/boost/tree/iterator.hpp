//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file   iterator.hpp
 * 
 * Iterator wrapper around a cursor, using a given type of traversal.
 */

#ifndef BOOST_TREE_ITERATOR_HPP
#define BOOST_TREE_ITERATOR_HPP

//#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace tree {

/**
 * @brief   Traversal order iterator adaptor
 * 
 *          Only works with ascending cursors.
 */
template <class Order, class Cursor>
class iterator
 : public boost::iterator_adaptor<iterator<Order, Cursor>
      , Cursor
      , boost::use_default
      , typename Order::iterator_category
    > {
 private:
    struct enabler {};

 public:
    iterator()
      : iterator::iterator_adaptor_() {}

    explicit iterator(Cursor p)
      : iterator::iterator_adaptor_(p) {}

    template <class OtherCursor>
    iterator(
        iterator<Order, OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor, Cursor>
          , enabler
        >::type = enabler()
    )
      : iterator::iterator_adaptor_(other.base()) {}

    operator Cursor()
    {
        return this->base();
    }
 private:
    friend class boost::iterator_core_access;
    
    void increment()
    {
        forward(Order(), this->base_reference());
        //BOOST_ASSERT(!this->base_reference().index() || this->base_reference().is_root());
    }
    
    void decrement()
    {
        back(Order(), this->base_reference());
        //BOOST_ASSERT(!this->base_reference().index() || this->base_reference().is_root());
    }
};

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

#endif //BOOST_TREE_ITERATOR_HPP