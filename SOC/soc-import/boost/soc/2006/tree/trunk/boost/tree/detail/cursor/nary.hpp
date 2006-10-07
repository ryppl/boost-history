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

// TODO: Use TR1 type_traits (integral_constant)
// can we abstract the cursor stuff any further, 
// eventually producing cursor_adaptor?

#ifndef BOOST_TREE_DETAIL_CURSOR_NARY_HPP
#define BOOST_TREE_DETAIL_CURSOR_NARY_HPP

#include <boost/tree/cursor_helpers.hpp>


namespace boost {
namespace tree {
namespace detail {

using boost::iterator_core_access;

using boost::tree::cursor_core_access;
using boost::tree::access_rotate;

template <class Node>
class nary_tree_cursor;

template<class Node>
class const_nary_tree_cursor
 : public cursor_facade<const_nary_tree_cursor<Node>
      , typename Node::value_type const //const is a hint for iterator_facade!
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
    struct enabler {};
	typedef const_nary_tree_cursor<Node> self_type;
	typedef typename self_type::cursor_facade_ cursor_facade_;
 public:
 	//TODO: Tidy up typedefs
 	 	
 	typedef Node const node_type;
	typedef node_type* node_pointer;

	typedef typename Node::base_pointer base_pointer;	
	typedef typename Node::const_base_pointer const_base_pointer;
	
	// Container-specific:
	typedef typename cursor_facade_::size_type size_type;

	// Cursor-specific
	//typedef integral_constant<size_type, 2> arity; // nary cursor
	
 	typedef nary_tree_cursor<Node> cursor;
 	typedef const_nary_tree_cursor<Node> const_cursor;

	//typedef bidirectional_traversal_tag vertical_traversal_type;
	
	typedef typename Node::metadata_type metadata_type;
	
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    const_nary_tree_cursor()
     : m_parent(0), m_pos(0) {}

    explicit const_nary_tree_cursor(
		const_base_pointer p, 
		size_type pos /*= 1*/)
     : m_parent(p), m_pos(pos) {}
      
    template <class OtherNode>
    const_nary_tree_cursor(
        const_nary_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, 
           	typename Node::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : m_parent(other.m_parent), m_pos(other.m_pos) {}


	const_base_pointer m_parent;
 	size_type m_pos;
 	
 private:
    friend class iterator_core_access;
    friend class cursor_core_access;
        
    typename Node::value_type const& dereference() const
	{
		return **static_cast<node_pointer>(m_parent->operator[](m_pos));
	}
	
    bool equal(const_nary_tree_cursor const& other) const
    {
		return (this->m_parent == other.m_parent) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
		++m_pos;
    }
    
    void decrement()
    {
		--m_pos;
    }
    
    void advance(typename const_nary_tree_cursor::cursor_facade_::difference_type n)
    {
    		m_pos += n;
    }
    
    typename const_nary_tree_cursor::cursor_facade_::difference_type
    distance_to(const_nary_tree_cursor z) const //TODO: convertible to instead of const_nary_tree_cursor (?)
    {
    		return (z.m_pos - this->m_pos);
    }
	
private:
	// Container-specific
	
	bool const empty_() const
	{
		return m_parent->operator[](m_pos)->empty();
	}
	
	size_type const size_() const
	{
		return m_parent->size();
	}
	
	size_type const max_size_() const
	{
		return m_parent->max_size();
	}
	
	// TODO (following couple of functions): wrap around node member fn
	const_cursor left() const
	{
		return const_cursor(m_parent->operator[](m_pos), 0); 
	}
		
	const_cursor right() const
	{
		return const_cursor(m_parent->operator[](m_pos), m_parent->size()-1);
	}

	// Cursor stuff. 	
	const_cursor up() const
	{
		return const_cursor(static_cast<const_base_pointer>(m_parent->parent()), m_parent->get_parity());
	}

public:

	size_type const parity() const
	{
		return m_pos;
	}

	metadata_type const& metadata() const
	{
		return static_cast<node_pointer>(m_parent->operator[](m_pos))->metadata();
	}
};

template <class Node> 
class nary_tree_cursor
 : public cursor_facade<nary_tree_cursor<Node>
      , typename Node::value_type
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
  	typedef typename Node::base_pointer base_pointer;
    struct enabler {};

 public:
 	typedef Node node_type;
	typedef node_type* node_pointer;

	// Container-specific:
	typedef typename node_type::size_type size_type;

	// Cursor-specific
	//typedef integral_constant<size_type, 2> arity; // nary cursor
		
 	typedef nary_tree_cursor<node_type> cursor;
 	typedef const_nary_tree_cursor<node_type> const_cursor;

	typedef bidirectional_traversal_tag vertical_traversal_type;
 
 	typedef typename node_type::metadata_type metadata_type;
 	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;
	
	template <class OtherValue>
	struct rebind {
		typedef nary_tree_cursor<OtherValue> other;
	};
	
    nary_tree_cursor()
      : m_parent(0), m_pos(0) {}

    explicit nary_tree_cursor(base_pointer p, size_type pos /*= 1*/)
      : m_parent(p), m_pos(pos) {}

    template <class OtherNode> //revisit
    nary_tree_cursor(
        nary_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, base_pointer>
          , enabler
        >::type = enabler()
    )
      : m_parent(other.m_parent), m_pos(other.m_pos) {}

 
 	base_pointer m_parent;
 	size_type m_pos;
 private:
 	friend class iterator_core_access;
    friend class cursor_core_access;
    
 	friend class access_rotate;
 	friend class access_detach;
 	
    typename node_type::reference dereference() const
	{
		return **static_cast<node_pointer>(m_parent->operator[](m_pos));
	}
	
    bool equal(cursor const& other) const
    {
        return (this->m_parent == other.m_parent) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
		++m_pos;
    }
    
    void decrement()
    {
		--m_pos;
    }    
    
	void advance(typename nary_tree_cursor::cursor_facade_::difference_type n)
    {
    		m_pos += n;
    }
    
    typename nary_tree_cursor::cursor_facade_::difference_type
    distance_to(nary_tree_cursor z) const //FIXME: convertible to instead of const_nary_tree_cursor
    {
    		return (z.m_pos - this->m_pos);
    }
    
	// Container specific
	bool const empty_() const
	{
		return m_parent->operator[](m_pos)->empty();
	}
	
	size_type size_()
	{
		return m_parent->size();
	}
	
	size_type max_size_()
	{
		return m_parent->max_size();
	}	
public:

	
	cursor begin()
	{
		return cursor(m_parent->operator[](m_pos), 0);
	}
	
	const_cursor left() const
	{
		return const_cursor(m_parent->operator[](m_pos), 0);
	}

	cursor end()
	{
		return cursor(m_parent->operator[](m_pos), m_parent->size()-1);
	}

	const_cursor right() const
	{
		return const_cursor(m_parent->operator[](m_pos), m_parent->size()-1);
	}
	
	// Cursor stuff
	cursor parent()
	{
		return cursor(static_cast<base_pointer>(m_parent->parent()), m_parent->get_parity());
	}
	
	const_cursor up() const
	{
		return const_cursor(static_cast<base_pointer>(m_parent->parent()), m_parent->get_parity());
	}
	
	node_pointer node() 
	{
		return static_cast<node_pointer>(m_parent->operator[](m_pos));
	}
	
	size_type const parity() const
	{
		return m_pos;
	}
		
protected:	
	void rotate()
	{
		m_pos = m_parent->rotate(m_pos);
		m_parent = static_cast<base_pointer>(m_parent->m_parent->m_parent);
	}
	

public:	
	// TODO: protect?
	void attach(node_pointer p_node)
	{
		p_node->m_parent = m_parent;
		
		// Only forest-relevant:
		p_node->operator[](1) = m_parent->operator[](m_pos);
		m_parent->operator[](m_pos)->m_parent = p_node;
		
		m_parent->operator[](m_pos) = p_node;
	}

	/** 
	 * Detaches the node this cursor points to and returns a pointer to it;
	 * this cursor will be set to its inorder successor afterwards (?)
	 */
	node_pointer detach()
	{
		return static_cast<node_pointer>(m_parent->detach(m_pos));
	}
	
	node_pointer detach(cursor y)
	{
		return static_cast<node_pointer>(m_parent->detach(m_pos, y.m_pos, y.m_parent));
	}
		
	metadata_type const& metadata() const
	{
		return static_cast<node_pointer>(m_parent->operator[](m_pos))->metadata();
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


#endif // BOOST_TREE_DETAIL_CURSOR_NARY_HPP
