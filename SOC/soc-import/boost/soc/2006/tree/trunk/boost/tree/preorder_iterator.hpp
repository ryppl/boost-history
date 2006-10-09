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

/**
 * @file preorder_iterator.hpp
 * Preorder iterator wrapper around cursor.
 */

// TODO: concept checks.

#ifndef BOOST_TREE_PREORDER_ITERATOR_HPP
#define BOOST_TREE_PREORDER_ITERATOR_HPP


#include <boost/tree/preorder.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {
namespace tree {
	
namespace preorder {

template <class Cursor, class Tag = typename cursor_category<Cursor>::type>
class iterator {};

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
 * @brief	First element of a MultiwayTree in preorder traversal
 * @param t	A MultiwayTree
 * @return	Mutable preorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> begin(MultiwayTree& t)
{
	return iterator<typename MultiwayTree::cursor>(first(t));
}

/**
 * @brief	First element of a MultiwayTree in preorder traversal
 * 			(Alias of cbegin())
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> begin(MultiwayTree const& t)
{
	return cbegin(t);
}

/**
 * @brief	First element of a MultiwayTree in preorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cbegin(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::const_cursor>(cfirst(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in preorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Mutable preorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> end(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::cursor>(last(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in preorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> end(MultiwayTree const& t)
{
	return cend(t);
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in preorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only preorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cend(MultiwayTree const& t)
{
	return iterator<typename MultiwayTree::const_cursor>(clast(t));
}

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_PREORDER_ITERATOR_HPP
