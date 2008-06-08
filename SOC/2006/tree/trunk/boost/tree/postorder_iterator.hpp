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

    explicit iterator(stack<Cursor> s) 
    		: m_s(s) {}
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
		return m_s.top();
	}
 private:
    friend class boost::iterator_core_access;

 	stack<Cursor> m_s;
    
    typename Cursor::value_type& dereference() const
    {
    		return *m_s.top();
    	}
    
    bool equal(iterator<Cursor, forward_traversal_tag> const& other) const
    {
    		return (this->m_s == other.m_s);
    }
    
    void increment()
    {
		m_s.pop();
		
		if (m_s.top().parity()) { // Right child? Return parent.
			--m_s.top();
			return;
		}
		
		if (m_s.size() == 1) // Root?
			return;
			
		// Left child.
		++m_s.top();
		while (!m_s.top().empty()) {
			m_s.push(m_s.top().begin());
			if (m_s.top().empty())
				++m_s.top();
		}
		if (m_s.top().parity())
			--m_s.top();
		return;
    }
    
    void decrement()
    {
		if (m_s.size() == 1) { // Root?
			m_s.push(m_s.top().begin());
			return;
		}
		
		if (!(++m_s.top()).empty()) { // Right
			m_s.push(m_s.top().begin());
			return;
		}
		if (!(--m_s.top()).empty()) { // Left
			m_s.push(m_s.top().begin());
			return;
		}
		
		// Move up in the hierarchy until we find a descendant that has a right
		// child (which is what we'll return) or we land at root.
		while (true) {
			m_s.pop();
			if (m_s.top().parity())
				if (!(--m_s.top()).empty()) {
					m_s.push(m_s.top().begin());
					return;
				}
		}
		return;
    }
};

#include <boost/tree/detail/iterator/bidirectional.hpp>


} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_POSTORDER_ITERATOR_HPP
