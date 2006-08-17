// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file iterators.hpp
 * Iterator wrappers around cursors.
 */

#ifndef BOOST_TREE_ITERATORS_HPP
#define BOOST_TREE_ITERATORS_HPP


#include <boost/tree/inorder.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {
namespace tree {
	
namespace inorder {

template <class Cursor>
class iterator
 : public boost::iterator_adaptor<iterator<Cursor>
      , Cursor
      , boost::use_default
      , boost::bidirectional_traversal_tag
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
        iterator<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor,Cursor >
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
		inorder::forward(this->base_reference());
    }
    
    void decrement()
    {
    		inorder::back(this->base_reference());
    }
};

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_ITERATORS_HPP
