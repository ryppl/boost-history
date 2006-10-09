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
 * @file inorder_iterator.hpp
 * Inorder iterator wrapper around cursor.
 */

// TODO: concept checks.

#ifndef BOOST_TREE_INORDER_ITERATOR_HPP
#define BOOST_TREE_INORDER_ITERATOR_HPP


#include <boost/tree/inorder.hpp>
#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <stack>


using std::stack;

namespace boost {
namespace tree {
	
namespace inorder {

template <class Cursor, class Tag = typename cursor_category<Cursor>::type>
class iterator {};

template <class Cursor>
class iterator<Cursor, forward_traversal_tag>
 : public boost::iterator_facade<iterator<Cursor, forward_traversal_tag>
      , typename Cursor::value_type
      , bidirectional_traversal_tag
    > {
// private:
//    struct enabler {};

 public:
    iterator() {}
//      : iterator::iterator_adaptor_() {}

    explicit iterator(stack<Cursor> s) //, bool branch = s.size()) // = s.size() 
    		: m_s(s), m_branch() {}
//      : iterator::iterator_adaptor_(p) {}

    explicit iterator(stack<Cursor> s, typename cursor_size<Cursor>::type branch)
    		: m_s(s), m_branch(branch) {}

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
		return m_s.top();
	}
 private:
    friend class boost::iterator_core_access;

 	stack<Cursor> m_s;
 	typename cursor_size<Cursor>::type m_branch;
    
    typename Cursor::value_type& dereference() const
    {
    		return *m_s.top();
    	}
    
    bool equal(iterator<Cursor, forward_traversal_tag> const& other) const
    {
    		return (this->m_s == other.m_s) && (this->m_branch == other.m_branch);
    }
    
    void increment()
    {
		if (!(++m_s.top()).empty()) {
			while (!m_s.top().begin().empty() && !m_s.top().end().empty())
				m_s.push(m_s.top().begin());
			if (!m_s.top().begin().empty() && m_s.top().end().empty())
				m_branch = m_s.size();
			while (!m_s.top().begin().empty())
				m_s.push(m_s.top().begin());	
			m_s.push(m_s.top().begin());
			return;
		}
		
		if (++m_branch == m_s.size())
			return;
			
		while (m_s.top().parity())
			m_s.pop();
		if (--m_branch > m_s.size()) 
			m_branch = m_s.size();
		return;
    }
    
    void decrement()
    {
	    	if (!m_s.top().empty()) {	    		
			while (!m_s.top().end().empty())
				m_s.push(m_s.top().end());
			m_s.push(m_s.top().begin());
			return;
		}
		while (!m_s.top().parity())
			m_s.pop();
	    	if (++m_branch > m_s.size()) 
			m_branch = m_s.size();
		--m_branch;
		--m_s.top();
		return;
    }
};

#include <boost/tree/detail/iterator/bidirectional.hpp>

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor, forward_traversal_tag> 
begin(MultiwayTree& t, forward_traversal_tag)
{
	typedef typename MultiwayTree::cursor cursor;
	std::stack<cursor> s;
 	typename cursor_size<cursor>::type branch = 0;
 	typename cursor_size<cursor>::type i = 0;
	s.push(t.root());
	while (!s.top().empty()) {
		++i;
		if (!branch && !s.top().end().empty())
			branch = i;
		s.push(s.top().begin());
	}
	return iterator<cursor, forward_traversal_tag>(s, branch);
}

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> 
begin(MultiwayTree& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::cursor>(first(t));
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::begin())
 * @param t	A MultiwayTree
 * @return	Mutable inorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> begin(MultiwayTree& t)
{
	typedef typename cursor_category<typename MultiwayTree::cursor>::type
		traversal;
	return begin(t, traversal());
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(Alias of cbegin(); equivalent to postorder::begin())
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> begin(MultiwayTree const& t)
{
	return cbegin(t);
}


template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor>
cbegin(MultiwayTree const& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::const_cursor>(cfirst(t));
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::begin())
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cbegin(MultiwayTree const& t)
{
	typedef typename cursor_category<
		typename MultiwayTree::const_cursor>::type traversal;
	return cbegin(t, traversal());
}

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor, forward_traversal_tag> 
end(MultiwayTree& t, forward_traversal_tag)
{
	typedef typename MultiwayTree::cursor cursor;
	std::stack<cursor> s;
	s.push(t.root());
	while (!s.top().empty())
		s.push(s.top().end());
	return iterator<cursor, forward_traversal_tag>(s, s.size());
}

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor>
end(MultiwayTree const& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::cursor>(last(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Mutable inorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> end(MultiwayTree& t)
{
	typedef typename cursor_category<typename MultiwayTree::cursor>::type
		traversal;
	return end(t, traversal());
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> end(MultiwayTree const& t)
{
	return cend(t);
}

template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor>
cend(MultiwayTree const& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::const_cursor>(clast(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cend(MultiwayTree const& t)
{
	typedef typename cursor_category<
		typename MultiwayTree::const_cursor>::type traversal;
	return cend(t, traversal());
}

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_ITERATOR_HPP
