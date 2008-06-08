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

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <boost/tree/cursor_helpers.hpp>

#include <stack>

namespace boost {
namespace tree {

template <class Cursor> 
class ascending_cursor
 : public cursor_facade<ascending_cursor<Cursor>
      , typename Cursor::value_type
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
    struct enabler {};

 public:
  	typedef typename Cursor::value_type value_type;

	// Container-specific:
	typedef typename Cursor::size_type size_type;

	// Cursor-specific
 	typedef ascending_cursor<Cursor> cursor;
 	typedef ascending_cursor<Cursor const> const_cursor; // FIXME
	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;
	
	template <class OtherValue>
	struct rebind {
		typedef ascending_cursor<OtherValue> other;
	};
	
    ascending_cursor()
      : m_s() {}

    explicit ascending_cursor(Cursor c)
    {
    	m_s.push(c); // Subtree root.
    }

    template <class OtherCursor>
    ascending_cursor(
        ascending_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, Cursor*>
          , enabler
        >::type = enabler()
    )
      : m_s(other.m_s) {}

 private: 

 	friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;
    
 	std::stack<Cursor> m_s; // pimpl?
 	
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

	// Cursor stuff
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
