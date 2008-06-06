//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder_iterator.hpp
 * Inorder iterator wrapper around cursor.
 */

// TODO: concept checks.

// NO guards, as this is context-included!

//#ifndef BOOST_TREE_DETAIL_ITERATOR_BIDIRECTIONAL_HPP
//#define BOOST_TREE_DETAIL_ITERATOR_BIDIRECTIONAL_HPP


//#include <boost/tree/inorder.hpp>
#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


//namespace boost {
//namespace tree {
//	
//namespace inorder {

/** @file	bidirectional.hpp
 * 
 * Some definitions that are identical for all *order cursors (as we are just
 * calling the appropriate traversal function that are defined in the 
 * respective *order.hpp files).
 */

/**
 * @brief	Specialisation for ascending cursors.
 */
template <class Cursor>
class iterator<Cursor, bidirectional_traversal_tag>
 : public boost::iterator_adaptor<iterator<Cursor, bidirectional_traversal_tag>
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
		forward(this->base_reference());
    }
    
    void decrement()
    {
    		back(this->base_reference());
    }
};


/// Iterators

template <class Cursor>
iterator<Cursor> 
begin(Cursor c, bidirectional_traversal_tag)
{
	return iterator<Cursor>(first(c));
}

/**
 * @brief	First element of a subtree in traversal
 * 			(equivalent to postorder::begin())
 * @param c	A cursor
 * @return	Iterator to the first element of @a c
 */
template <class Cursor>
iterator<Cursor> begin(Cursor c)
{
	typedef
		typename cursor_vertical_traversal<Cursor>::type
		traversal;
	return begin(c, traversal());
}


template <class Cursor>
iterator<Cursor, bidirectional_traversal_tag>
end(Cursor c, bidirectional_traversal_tag)
{
	return iterator<Cursor>(last(c));
}

/**
 * @brief	One position past the last element of a subtree 
 * 			in traversal (Alias of cend())
 * @param c	A cursor
 * @return	Iterator one position past the last element of @a c 
 */
template <class Cursor>
iterator<Cursor> end(Cursor c)
{
	typedef
		typename cursor_vertical_traversal<Cursor>::type
		traversal;
	return end(c, traversal());
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

//#endif // BOOST_TREE_DETAIL_ITERATOR_BIDIRECTIONAL_HPP
