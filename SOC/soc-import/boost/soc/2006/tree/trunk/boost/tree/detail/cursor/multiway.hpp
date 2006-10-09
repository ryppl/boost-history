//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// TODO: Use TR1 type_traits (integral_constant)

#ifndef BOOST_TREE_DETAIL_CURSOR_MULTIWAY_HPP
#define BOOST_TREE_DETAIL_CURSOR_MULTIWAY_HPP

#include <boost/tree/detail/cursor/nary.hpp>

#include <boost/tree/cursor.hpp>
#include <boost/tree/cursor_helpers.hpp>

//#include <boost/type_traits/integral_constant.hpp>
//
//#include <boost/type_traits/is_convertible.hpp>
//#include <boost/utility/enable_if.hpp>
//
//#include <boost/iterator/iterator_adaptor.hpp>


#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {

template <class Cursor>
class multiway_tree_cursor;

template<class Cursor>
class const_multiway_tree_cursor
 : public cursor_adaptor<const_multiway_tree_cursor<Cursor>
      , Cursor const
      , boost::use_default
      , bidirectional_traversal_tag
      , forward_traversal_tag
    > {
 private:
    struct enabler {};

 public:
 	//TODO: Tidy up typedefs

	typedef Cursor base_cursor;
	
 	typedef multiway_tree_cursor<Cursor> cursor;
 	typedef const_multiway_tree_cursor<Cursor> const_cursor;

	typedef typename cursor_size<base_cursor>::type size_type;

	typedef bidirectional_traversal_tag cursor_category;
	
	typedef typename Cursor::metadata_type metadata_type;
	
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    const_multiway_tree_cursor()
     : const_multiway_tree_cursor::cursor_adaptor_() {}

    explicit const_multiway_tree_cursor(base_cursor p)
     : const_multiway_tree_cursor::cursor_adaptor_(p) {}
      
    template <class OtherCursor>
    const_multiway_tree_cursor(
        const_multiway_tree_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, 
           	typename Cursor::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : const_multiway_tree_cursor::cursor_adaptor_(other.base()) {}

	operator base_cursor()
	{
		return this->base();
	}

 private:
	
    friend class boost::iterator_core_access;
    
	typename const_multiway_tree_cursor::cursor_adaptor_::reference dereference() const
	{
		return this->base()->m_parent->operator[](this->parity());
	}
};

template <class Cursor> 
class multiway_tree_cursor
 : public cursor_adaptor<multiway_tree_cursor<Cursor>
      , Cursor
      , boost::use_default
      , bidirectional_traversal_tag
      , forward_traversal_tag
    > {
 private:
    struct enabler {};
	friend class cursor_core_access;
 public:
 	//TODO: Tidy up typedefs

	typedef Cursor base_cursor;
	
 	typedef multiway_tree_cursor<Cursor> cursor;
 	typedef const_multiway_tree_cursor<Cursor> const_cursor;

	typedef typename cursor_size<base_cursor>::type size_type;

	//typedef bidirectional_traversal_tag cursor_category;
	
	typedef typename base_cursor::metadata_type metadata_type;
	
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    multiway_tree_cursor()
     : multiway_tree_cursor::cursor_adaptor_() {}

    explicit multiway_tree_cursor(base_cursor p)
     : multiway_tree_cursor::cursor_adaptor_(p) {}
      
    template <class OtherCursor>
    multiway_tree_cursor(
        multiway_tree_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, 
           	typename Cursor::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : multiway_tree_cursor::cursor_adaptor_(other.base()) {}

	operator base_cursor()
	{
		return this->base();
	}


 private:
	
    friend class boost::iterator_core_access;
	
	typename multiway_tree_cursor::cursor_adaptor_::reference dereference() const
	{
		return this->base()->m_parent->operator[](this->parity());
	}
	
public:
	// Range stuff.
//	cursor begin()
//	{
//		return cursor(this->base_reference().begin()); 
//	}
	
//	const_cursor begin() const
//	{
//		return const_cursor(this->base_reference().begin()); 
//	}		

	// Cursor stuff. 
	
//	const_cursor parent() const
//	{
//		if (!this->base_reference().parity()) {
//			return this->base_reference().parent();
//		}
//	}
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_CURSOR_MULTIWAY_HPP
