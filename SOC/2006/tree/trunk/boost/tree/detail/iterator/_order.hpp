//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file	_order.hpp
 * 
 * Some definitions that are identical for all *order cursors (as we are just
 * calling the appropriate traversal function that are defined in the 
 * respective *order.hpp files).
 */

// TODO: concept checks.

// NO guards, as this is context-included!

//#include <boost/tree/cursor.hpp>

/**
 * @brief	Traversal order iterator adaptor
 * 
 *			Only works with ascending cursors.
 */
 
template <class Cursor, class RootTracker = typename Cursor::root_tracker>
class iterator
 : public boost::iterator_adaptor<iterator<Cursor>
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
        iterator<OtherCursor> const& other
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
