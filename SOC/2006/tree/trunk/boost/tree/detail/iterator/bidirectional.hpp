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


//template <class MultiwayTree>
//iterator<typename MultiwayTree::cursor> 
//begin(MultiwayTree& t, bidirectional_traversal_tag)
//{
//	return iterator<typename MultiwayTree::cursor>(first(t));
//}
//
///**
// * @brief	First element of a MultiwayTree in inorder traversal
// * 			(equivalent to postorder::begin())
// * @param t	A MultiwayTree
// * @return	Mutable inorder iterator to the first element of @a t
// */
//template <class MultiwayTree>
//iterator<typename MultiwayTree::cursor> begin(MultiwayTree& t)
//{
//	typedef typename cursor_category<typename MultiwayTree::cursor>::type
//		traversal;
//	return begin(t, traversal());
//}
//
///**
// * @brief	First element of a MultiwayTree in inorder traversal
// * 			(Alias of cbegin(); equivalent to postorder::begin())
// * @param t	A MultiwayTree
// * @return	Read-only inorder iterator to the first element of @a t
// */
//template <class MultiwayTree>
//iterator<typename MultiwayTree::const_cursor> begin(MultiwayTree& t)
//{
//	return cbegin(t);
//}
//
//
//template <class MultiwayTree>
//iterator<typename MultiwayTree::const_cursor>
//cbegin(MultiwayTree& t, bidirectional_traversal_tag)
//{
//	return iterator<typename MultiwayTree::const_cursor>(cfirst(t));
//}
//
///**
// * @brief	First element of a MultiwayTree in inorder traversal
// * 			(equivalent to postorder::begin())
// * @param t	A MultiwayTree
// * @return	Read-only inorder iterator to the first element of @a t
// */
//template <class MultiwayTree>
//iterator<typename MultiwayTree::const_cursor> cbegin(MultiwayTree& t)
//{
//	typedef typename cursor_category<
//		typename MultiwayTree::const_cursor>::type traversal;
//	return cbegin(t, traversal());
//}
//
//
//template <class MultiwayTree>
//iterator<typename MultiwayTree::cursor>
//end(MultiwayTree const& t, bidirectional_traversal_tag)
//{
//	return iterator<typename MultiwayTree::cursor>(last(t));
//}
//
///**
// * @brief	One position past the last element of a MultiwayTree 
// * 			in inorder traversal (Alias of cend())
// * @param t	A MultiwayTree
// * @return	Mutable inorder iterator one position past the last element of @a t 
// */
//template <class MultiwayTree>
//iterator<typename MultiwayTree::cursor> end(MultiwayTree& t)
//{
//	typedef typename cursor_category<typename MultiwayTree::cursor>::type
//		traversal;
//	return end(t, traversal());
//}
//
///**
// * @brief	One position past the last element of a MultiwayTree 
// * 			in inorder traversal (Alias of cend())
// * @param t	A MultiwayTree
// * @return	Read-only inorder iterator one position past the last element of @a t 
// */
//template <class MultiwayTree>
//iterator<typename MultiwayTree::const_cursor> end(MultiwayTree const& t)
//{
//	return cend(t);
//}
//
//template <class MultiwayTree>
//iterator<typename MultiwayTree::const_cursor>
//cend(MultiwayTree const& t, bidirectional_traversal_tag)
//{
//	return iterator<typename MultiwayTree::const_cursor>(clast(t));
//}
//
///**
// * @brief	One position past the last element of a MultiwayTree 
// * 			in inorder traversal
// * @param t	A MultiwayTree
// * @return	Read-only inorder iterator one position past the last element of @a t 
// */
//template <class MultiwayTree>
//iterator<typename MultiwayTree::const_cursor> cend(MultiwayTree& t)
//{
//	typedef typename cursor_category<
//		typename MultiwayTree::const_cursor>::type traversal;
//	return cend(t, traversal());
//}

//} // namespace inorder
//
//} // namespace tree
//} // namespace boost

//#endif // BOOST_TREE_DETAIL_ITERATOR_BIDIRECTIONAL_HPP
