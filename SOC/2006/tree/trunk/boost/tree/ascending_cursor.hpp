//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file ascending.hpp
 * Ascending cursor adaptor implementation
 */

#ifndef BOOST_TREE_ASCENDING_CURSOR_HPP
#define BOOST_TREE_ASCENDING_CURSOR_HPP


#include <boost/tree/cursor.hpp>
#include <boost/tree/cursor_helpers.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <stack>

namespace boost {
namespace tree {

template <class DescendingCursor> 
class ascending_cursor
 : public cursor_facade<ascending_cursor<DescendingCursor>
      , typename cursor_value<DescendingCursor>::type
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
    struct enabler {};

 public:
  	typedef typename DescendingCursor::value_type value_type;

	// Container-specific:
	typedef typename DescendingCursor::size_type size_type;

	// DescendingCursor-specific
 	typedef ascending_cursor<DescendingCursor> cursor;
 	typedef ascending_cursor<typename DescendingCursor::const_cursor> const_cursor;
	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;
	
	template <class OtherCursor>
	struct rebind {
		typedef ascending_cursor<OtherCursor> other;
	};
	
    ascending_cursor()
      : m_s() {}

    explicit ascending_cursor(DescendingCursor c)
    {
    	m_s.push(c); // Subtree root.
    }

    template <class OtherCursor>
    ascending_cursor(
        ascending_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, DescendingCursor*>
          , enabler
        >::type = enabler()
    )
      : m_s(other.m_s) {}

	struct root_tracker {
		root_tracker() {}
		root_tracker& operator++()
		{
			return *this;
		}
		
		root_tracker& operator--()
		{
			return *this;
		}
		
		bool is_root(cursor c)
		{
			return (c.is_root());
		} 
	};

 private: 

 	friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;
    
 	std::stack<DescendingCursor> m_s; // pimpl?
 	
    value_type& dereference() const
	{
		return *m_s.top();
	}
	
    bool equal(cursor const& other) const
    {
        return (this->m_s == other.m_s);
    }
    
    void increment()
    {
		++m_s.top();
    }
    
    void decrement()
    {
		--m_s.top();
    }    
    
	void advance(typename ascending_cursor::cursor_facade_::difference_type n)
    {
    		m_s.top() += n;
    }
    
    typename ascending_cursor::cursor_facade_::difference_type
    distance_to(ascending_cursor z) const //FIXME: convertible to instead of ascending_cursor
    {
    		return (z.m_s.top() - this->m_s.top());
    }
    
	// Container specific
	bool const empty_() const
	{
		return m_s.top().empty();
	}
	
	size_type size_()
	{
		return m_s.top().size();
	}
	
	size_type max_size_()
	{
		return m_s.top().max_size();
	}
	
	size_type const par() const
	{
		return m_s.top().parity();
	}

	void left()
	{
		m_s.push(m_s.top().begin());
	}

	void right()
	{
		m_s.push(m_s.top().end());
	}

	// DescendingCursor stuff
	void up()
	{
		m_s.pop();
	}

public:
	bool is_root() const
	{
		return (m_s.size() == 1);
	}
};


} // namespace tree
} // namespace boost


#endif // BOOST_TREE_ASCENDING_CURSOR_HPP
