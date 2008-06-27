//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file forest.hpp
 * Forest cursor template
 */

// TODO: Use TR1 type_traits (integral_constant)
// can we abstract the cursor stuff any further, 
// eventually producing cursor_adaptor?

#ifndef BOOST_TREE_DETAIL_CURSOR_FOREST_HPP
#define BOOST_TREE_DETAIL_CURSOR_FOREST_HPP

#include <boost/tree/cursor.hpp>
#include <boost/tree/cursor_adaptor.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {

using boost::tree::cursor_core_access;
using boost::iterator_core_access;

template <class Cursor> 
class forest_cursor
: public cursor_adaptor<forest_cursor<Cursor>
      , Cursor
      , boost::use_default
      , bidirectional_traversal_tag
      , forward_traversal_tag
    > {
private:
    struct enabler {};

public:
 	//TODO: Tidy up typedefs

	typedef Cursor base_cursor;
	
 	typedef forest_cursor<Cursor> cursor;
 	//typedef const_forest_cursor<Cursor> const_cursor;

	//typedef typename cursor_size<base_cursor>::type size_type;

	//typedef bidirectional_traversal_tag cursor_category;
		
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
//	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    forest_cursor()
     : forest_cursor::cursor_adaptor_() {}

    explicit forest_cursor(base_cursor p)
     : forest_cursor::cursor_adaptor_(p) {}
      
    template <class OtherCursor>
    forest_cursor(
        forest_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, 
           	typename Cursor::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : forest_cursor::cursor_adaptor_(other.base()) {}

	operator base_cursor()
	{
		return this->base();
	}
	
private:
	
    friend class cursor_core_access;
    friend class iterator_core_access;

//	bool empty_() const
//	{ 
//	}

	void increment()
    {
		 if (!(++this->base_reference()).empty())
		 	this->base_reference().to_begin();
    }
    
    void decrement()
    {
    	if (!this->base_reference().parity())
			this->base_reference().to_parent();
    	--this->base_reference();
    }
	
	// Range stuff.
	
	// left() remains unchanged, so no need to re-implement it.

	void right()
	{
		while (!this->base_reference().to_end().empty());
	}
	
	// Cursor stuff. 
	
	void up()
	{
		if (!this->base_reference().parity())
			this->base_reference().to_parent();
	}
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_CURSOR_FOREST_HPP
