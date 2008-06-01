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

template <class Cursor, class Tag = typename cursor_category<Cursor>::type>
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


} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INORDER_ITERATOR_HPP
