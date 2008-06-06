//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file postorder_iterator.hpp
 * Postorder iterator wrapper around cursor.
 */

// TODO: concept checks.

#ifndef BOOST_TREE_POSTORDER_ITERATOR_HPP
#define BOOST_TREE_POSTORDER_ITERATOR_HPP


#include <boost/tree/postorder.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {
namespace tree {
	
namespace postorder {

template <class Cursor, 
		  class Tag = typename cursor_vertical_traversal<Cursor>::type>
class iterator;

// TODO. For now, it's only a (bidirectional) dummy
template <class Cursor>
class iterator<Cursor, forward_traversal_tag>
 : public boost::iterator_facade<iterator<Cursor, forward_traversal_tag>
      , typename Cursor::value_type
      , bidirectional_traversal_tag
    > {
 private:
    struct enabler {};

 public:
    iterator() {}
//      : iterator::iterator_adaptor_() {}

    explicit iterator(Cursor p) {}
//      : iterator::iterator_adaptor_(p) {}

//    template <class OtherCursor>
//    iterator(
//        iterator<OtherCursor> const& other
//      , typename boost::enable_if<
//            boost::is_convertible<OtherCursor,Cursor >
//          , enabler
//        >::type = enabler()
//    )
//      : iterator::iterator_adaptor_(other.base()) {}

	operator Cursor()
	{
		return this->base();
	}
 private:
    friend class boost::iterator_core_access;
    
    void increment()
    {
		forward(this->base_reference()); //Dummy
    }
    
    void decrement()
    {
    	back(this->base_reference()); //Dummy
    }
};

#include <boost/tree/detail/iterator/bidirectional.hpp>


} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_POSTORDER_ITERATOR_HPP
