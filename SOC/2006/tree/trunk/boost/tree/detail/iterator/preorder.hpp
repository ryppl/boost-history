//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder_iterator.hpp
 * Preorder iterator wrapper around cursor.
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ITERATOR_PREORDER_HPP
#define BOOST_TREE_DETAIL_ITERATOR_PREORDER_HPP


#include <boost/tree/detail/cursor/preorder.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {
namespace tree {
	
namespace preorder {

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
		// If we have a left child, go there.
		if (!m_s.top().empty()) {
			m_s.push(m_s.top().begin());
			return;
		}
		
		// No left child. So if we have a right child, go there.
		if (!(++m_s.top()).empty()) {
			m_s.push(m_s.top().begin());
			return;
		}
		
		// (Here's where we'd need to check if this is the end().)
		
		// No children at all.
		// As we've already visited all the ancestors, we'll move upwards until
		// we find an ancestor that has a right child.
		while (true) { // Doesn't work with subtrees!
			if (m_s.size() == 1) // Root
				return;

			m_s.pop();
			if (!m_s.top().parity()) {
				if (m_s.size() == 1) // Root?
					return;
				if (!(++m_s.top()).empty()) {
					m_s.push(m_s.top().begin());
					return;
				}
			}
		}
		return;
}
    
    void decrement()
    {
		if (m_s.size() != 1) { // Not root
			m_s.pop();
			
			// If this is a left child, just move to its parent.
			if (!m_s.top().parity()) {
				m_s.pop();
				m_s.push(m_s.top().begin());
				return;
			}
			
			// So this is a right child.
			--m_s.top();
		}
		
		// Same for root (=end) and right children:
		if (!m_s.top().empty())
			while (!m_s.top().empty())
				if (!m_s.top().end().empty())
					m_s.push(m_s.top().end());
				else
					m_s.push(m_s.top().begin());
		return;
    }
};

#include <boost/tree/detail/iterator/bidirectional.hpp>


} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ITERATOR_PREORDER_HPP
