//  Copyright (c) 2006, Bernhard Reiter
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

template <class Cursor, class Tag = typename cursor_category<Cursor>::type>
class iterator;

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
		forward(this->base_reference());
    }
    
    void decrement()
    {
    		back(this->base_reference());
    }
};

#include <boost/tree/detail/iterator/bidirectional.hpp>


/**
 * @brief	First element of a MultiwayTree in postorder traversal
 * 			(equivalent to inorder::begin())
 * @param t	A MultiwayTree
 * @return	Mutable postorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> begin(MultiwayTree& t)
{
	return iterator<typename MultiwayTree::cursor>(first(t));
}

/**
 * @brief	First element of a MultiwayTree in postorder traversal
 * 			(Alias of cbegin(); equivalent to inorder::begin())
 * @param t	A MultiwayTree
 * @return	Read-only postorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> begin(MultiwayTree& t)
{
	return iterator<typename MultiwayTree::const_cursor>(first(t));
}

/**
 * @brief	First element of a MultiwayTree in postorder traversal
 * 			(equivalent to inorder::begin())
 * @param t	A MultiwayTree
 * @return	Read-only postorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cbegin(MultiwayTree& t)
{
	return cbegin(t);
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in postorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Mutable postorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> end(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::cursor>(last(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in postorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Read-only postorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> end(MultiwayTree const& t)
{
	return cend(t);
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in postorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only postorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cend(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::const_cursor>(clast(t));
}

} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_POSTORDER_ITERATOR_HPP
