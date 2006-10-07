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
 * @file nary.hpp
 * Nary cursor implementation
 */

#ifndef BOOST_TREE_DETAIL_CURSOR_MULTIWAY_HPP
#define BOOST_TREE_DETAIL_CURSOR_MULTIWAY_HPP

#include <boost/tree/cursor_helpers.hpp>


namespace boost {
namespace tree {
namespace detail {

using boost::iterator_core_access;

using boost::tree::cursor_core_access;
using boost::tree::access_rotate;

template <class Cursor>
class multiway_tree_cursor;


template<class Cursor>
class const_multiway_tree_cursor
 : public cursor_facade<const_multiway_tree_cursor<Cursor>
      , typename Cursor::value_type const //const is a hint for iterator_facade!
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
    struct enabler {};
	typedef const_multiway_tree_cursor<Cursor> self_type;
	typedef typename self_type::cursor_facade_ cursor_facade_;
 public:
 	//TODO: Tidy up typedefs
 	 	
 	typedef Cursor base_cursor;

//	typedef typename Cursor::base_pointer base_pointer;	
//	typedef typename Cursor::const_base_pointer const_base_pointer;
	
	// Container-specific:
	typedef typename cursor_facade_::size_type size_type;

	// Cursor-specific
	
 	typedef multiway_tree_cursor<Cursor> cursor;
 	typedef const_multiway_tree_cursor<Cursor> const_cursor;
	
	typedef typename Cursor::metadata_type metadata_type;
	
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    const_multiway_tree_cursor()
     : m_cur(0), m_pos(0) {}

    explicit const_multiway_tree_cursor(base_cursor p, size_type pos)
     : m_cur(p), m_pos(pos) {}
      
    template <class OtherCursor>
    const_multiway_tree_cursor(
        const_multiway_tree_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, 
           	typename Cursor::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : m_cur(other.m_cur), m_pos(other.m_pos) {}

	

	base_cursor m_cur;
 	size_type m_pos;

 private:
    friend class iterator_core_access;
    friend class cursor_core_access;
        
    //const
    typename base_cursor::reference dereference() const
	{
		//return 
		//return helper<base_cursor, const_base_pointer, size_type>::deref(m_cur, m_pos);
	}
	
    bool equal(const_multiway_tree_cursor const& other) const
    {
		return (this->m_cur == other.m_cur) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
		++m_pos;
    }
    
    void decrement()
    {
		--m_pos;
    }
    
    void advance(typename const_multiway_tree_cursor::cursor_facade_::difference_type n)
    {
    		m_pos += n;
    }
    
    typename const_multiway_tree_cursor::cursor_facade_::difference_type
    distance_to(const_multiway_tree_cursor z) const //TODO: convertible to instead of const_multiway_tree_cursor (?)
    {
    		return (z.m_pos - this->m_pos);
    }
	
private:
	// Container-specific
	
	bool const empty_() const
	{
		return m_cur->empty();
	}
	
	size_type const size_() const
	{
		return m_cur->size();
	}
	
	size_type const max_size_() const
	{
		return m_cur->max_size();
	}
	
	// TODO (following couple of functions): wrap around cursor member fn
	const_cursor left() const
	{
		return const_cursor(m_cur->begin());
	}
		
	const_cursor right() const
	{
		return const_cursor(m_cur->end());
	}

	// Cursor stuff. 	
	const_cursor up() const
	{
		return const_cursor(m_cur->parent());
	}

public:

	size_type const parity() const
	{
		return m_pos;
	}

	metadata_type const& metadata() const
	{
		return m_cur->metadata();
	}
};

template <class Cursor> 
class multiway_tree_cursor
 : public cursor_facade<multiway_tree_cursor<Cursor>
      , typename Cursor::value_type
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
  	//typedef typename Cursor::base_pointer base_pointer;
    struct enabler {};

 public:
 	typedef Cursor base_cursor;
	typedef base_cursor* cursor_pointer;

	// Container-specific:
	typedef typename base_cursor::size_type size_type;

	// Cursor-specific
		
 	typedef multiway_tree_cursor<base_cursor> cursor;
 	typedef const_multiway_tree_cursor<base_cursor> const_cursor;

 	typedef typename base_cursor::metadata_type metadata_type;
 	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;
	
	template <class OtherValue>
	struct rebind {
		typedef multiway_tree_cursor<OtherValue> other;
	};
	
    multiway_tree_cursor()
      : m_cur(0), m_pos(0) {}

    explicit multiway_tree_cursor(base_cursor p, size_type pos)
    : m_cur(p), m_pos(pos) {}

    template <class OtherCursor> //revisit
    multiway_tree_cursor(
        multiway_tree_cursor<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*, base_cursor>
          , enabler
        >::type = enabler()
    )
      : m_cur(other.m_cur), m_pos(other.m_pos) {}

 	base_cursor m_cur;
 	size_type m_pos;

 private: 

 	friend class iterator_core_access;
    friend class cursor_core_access;
    
 	friend class access_rotate;
 	friend class access_detach;
 	
    typename base_cursor::reference dereference() const
	{
		//return helper<base_cursor, base_pointer, size_type>::deref(m_cur, m_pos);
	}
	
    bool equal(cursor const& other) const
    {
        return (this->m_cur == other.m_cur) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
		++m_pos;
    }
    
    void decrement()
    {
		--m_pos;
    }    
    
	void advance(typename multiway_tree_cursor::cursor_facade_::difference_type n)
    {
    		m_pos += n;
    }
    
    typename multiway_tree_cursor::cursor_facade_::difference_type
    distance_to(multiway_tree_cursor z) const //FIXME: convertible to instead of const_multiway_tree_cursor
    {
    		return (z.m_pos - this->m_pos);
    }
    
	// Container specific
	bool const empty_() const
	{
		return m_cur->operator[](m_pos)->empty();
	}
	
	size_type size_()
	{
		return m_cur->size();
	}
	
	size_type max_size_()
	{
		return m_cur->max_size();
	}	
public:

	
	cursor begin()
	{
		return cursor(m_cur->begin());
	}
	
	const_cursor left() const
	{
		return const_cursor(m_cur->begin());
	}

	cursor end()
	{
		return cursor(m_cur->end());
	}

	const_cursor right() const
	{
		return const_cursor(m_cur->end());
	}
	
	// Cursor stuff
	cursor parent()
	{
		return cursor(m_cur->parent());
	}
	
	const_cursor up() const
	{
		return const_cursor(m_cur->parent());
	}
	
	size_type const parity() const
	{
		return m_pos;
	}
		

	
//	bool const is_root() const
//	{
//		return ((m_cur->m_cur->operator[](0) != m_cur) 
//			 && (m_cur->m_cur->operator[](1) != m_cur))
//			 ||  m_cur->m_cur == m_cur->operator[](1); // empty root
//	}

//protected:	
//	void rotate()
//	{
//		m_pos = m_cur->rotate(m_pos);
//		m_cur = static_cast<base_pointer>(m_cur->m_cur->m_cur);
//	}
	

public:	
	// TODO: protect?
	void attach(cursor_pointer p_cursor)
	{
		p_cursor->m_cur = m_cur;
		
		// Only forest-relevant:
		p_cursor->operator[](1) = m_cur->operator[](m_pos);
		m_cur->operator[](m_pos)->m_cur = p_cursor;
		
		m_cur->operator[](m_pos) = p_cursor;
	}

	/** 
	 * Detaches the cursor this cursor points to and returns a pointer to it;
	 * this cursor will be set to its inorder successor afterwards (?)
	 */
	cursor_pointer detach()
	{
		return static_cast<cursor_pointer>(m_cur->detach(m_pos));
	}
	
	cursor_pointer detach(cursor y)
	{
		return static_cast<cursor_pointer>(m_cur->detach(m_pos, y.m_pos, y.m_cur));
	}
		
	metadata_type const& metadata() const
	{
		return m_cur->metadata();
	}
	
	metadata_type& metadata()
	{
		return const_cast<metadata_type&>
			  (static_cast<cursor const*>(this)->metadata());
	}
};


} // namespace detail
} // namespace tree
} // namespace boost


#endif // BOOST_TREE_DETAIL_CURSOR_MULTIWAY_HPP
