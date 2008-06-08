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

template <class MultiwayCursor, 
		  class Tag = typename cursor_vertical_traversal<MultiwayCursor>::type>
class iterator;

template <class MultiwayCursor>
class iterator<MultiwayCursor, forward_traversal_tag>
 : public boost::iterator_facade<iterator<MultiwayCursor, forward_traversal_tag>
      , typename MultiwayCursor::value_type
      , bidirectional_traversal_tag
    > {
// private:
//    struct enabler {};

 public:
    iterator() {}
//      : iterator::iterator_adaptor_() {}

    explicit iterator(stack<MultiwayCursor> s) 
    		: m_s(s) {}
//      : iterator::iterator_adaptor_(p) {}

//    template <class OtherMultiwayCursor>
//    iterator(
//        iterator<OtherMultiwayCursor> const& other
//      , typename boost::enable_if<
//            boost::is_convertible<OtherMultiwayCursor,MultiwayCursor >
//          , enabler
//        >::type = enabler()
//    )
//      : iterator::iterator_adaptor_(other.base()) {}

	operator MultiwayCursor()
	{
		return m_s.top();
	}
 private:
    friend class boost::iterator_core_access;

 	stack<MultiwayCursor> m_s;
    
    typename MultiwayCursor::value_type& dereference() const
    {
    		return *m_s.top();
    	}
    
    bool equal(iterator<MultiwayCursor, forward_traversal_tag> const& other) const
    {
    		return (this->m_s == other.m_s);
    }
    
    void increment()
    {
		if (!(++m_s.top()).empty()) {
			while (!m_s.top().begin().empty())
				m_s.push(m_s.top().begin());
			m_s.push(m_s.top().begin());
			return;
		}
		
		while (m_s.top().parity() && !m_s.empty())
			m_s.pop();
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
		--m_s.top();
		return;
    }
};

#include <boost/tree/detail/iterator/bidirectional.hpp>


} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_ITERATOR_HPP
