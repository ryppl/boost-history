//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file   iterator.hpp
 * 
 * Some definitions that are identical for all *order cursors (as we are just
 * calling the appropriate traversal function that are defined in the 
 * respective *order.hpp files).
 */

#ifndef BOOST_TREE_DETAIL_ITERATOR_ITERATOR_HPP
#define BOOST_TREE_DETAIL_ITERATOR_ITERATOR_HPP

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
      , bidirectional_traversal_tag
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
        forward(this->base_reference());
        //BOOST_ASSERT(!this->base_reference().parity() || this->base_reference().is_root());
    }
    
    void decrement()
    {
        back(this->base_reference());
        //BOOST_ASSERT(!this->base_reference().parity() || this->base_reference().is_root());
    }
};

} // namespace tree
} // namespace boost

#endif //BOOST_TREE_DETAIL_ITERATOR_ITERATOR_HPP