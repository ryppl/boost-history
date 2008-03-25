//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file nary.hpp
 * Nary cursor implementation
 */

#ifndef BOOST_TREE_DETAIL_CURSOR_NARY_HPP
#define BOOST_TREE_DETAIL_CURSOR_NARY_HPP

#include <boost/tree/cursor_helpers.hpp>


namespace boost {
namespace tree {
namespace detail {

using boost::iterator_core_access;

using boost::tree::cursor_core_access;

template <class Node>
class nary_tree_cursor;

template <class Node, class BasePtr, class SizeType>
class helper {
 public:
	static typename Node::reference deref(BasePtr par, SizeType pos)
	{
		return **static_cast<Node*>(par->operator[](pos));
	}
};


template <class BasePtr, class SizeType, class Value>
class helper<node<Value, detail::binary_array> const, BasePtr, SizeType > {
	typedef node<Value, detail::binary_array> const node_type;
 public:
	static typename node_type::const_reference deref(BasePtr par, SizeType pos)
	{
		return **static_cast<node_type*>(par);
	}
};

template <class BasePtr, class SizeType, class Value>
class helper<node<Value, detail::binary_array>, BasePtr, SizeType > {
	typedef node<Value, detail::binary_array> node_type;
 public:
	static typename node_type::reference deref(BasePtr par, SizeType pos)
	{
		return **static_cast<node_type*>(par);
	}
};

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
	
 	typedef nary_tree_cursor<Node> cursor;
 	typedef const_nary_tree_cursor<Node> const_cursor;
	
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    const_nary_tree_cursor()
     : m_node(0), m_pos(0) {}

    explicit const_nary_tree_cursor(const_base_pointer p, size_type pos)
     : m_node(p), m_pos(pos) {}
      
    template <class OtherNode>
    const_nary_tree_cursor(
        const_nary_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, 
           	typename Node::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : m_node(other.m_node), m_pos(other.m_pos) {}

	const_base_pointer m_node;
 	size_type m_pos;

 private:
    friend class iterator_core_access;
    friend class cursor_core_access;

    typename node_type::const_reference dereference() const
	{
		return helper<node_type, const_base_pointer, size_type>::deref(m_node, m_pos);
	}
	
    bool equal(const_nary_tree_cursor const& other) const
    {
		return (this->m_node == other.m_node) && (this->m_pos == other.m_pos);
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
		return m_node->operator[](m_pos)->empty();
	}
	
	size_type const size_() const
	{
		return m_node->size();
	}
	
	size_type const max_size_() const
	{
		return m_node->max_size();
	}
	
	// TODO (following couple of functions): wrap around node member fn
	const_cursor left() const
	{
		return const_cursor(m_node->operator[](m_pos), 0); 
	}
		
	const_cursor right() const
	{
		return const_cursor(m_node->operator[](m_pos), m_node->size()-1);
	}

	// Cursor stuff. 	
	const_cursor up() const
	{
		return const_cursor(static_cast<const_base_pointer>(m_node->parent()), m_node->get_parity());
	}

	size_type const par() const
	{
		return m_pos;
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
		
 	typedef nary_tree_cursor<node_type> cursor;
 	typedef const_nary_tree_cursor<node_type> const_cursor;
	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;
	
	template <class OtherValue>
	struct rebind {
		typedef nary_tree_cursor<OtherValue> other;
	};
	
    nary_tree_cursor()
      : m_node(0), m_pos(0) {}

    explicit nary_tree_cursor(base_pointer p, size_type pos)
    : m_node(p), m_pos(pos) {}

    template <class OtherNode> //revisit
    nary_tree_cursor(
        nary_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, base_pointer>
          , enabler
        >::type = enabler()
    )
      : m_node(other.m_node), m_pos(other.m_pos) {}

 	base_pointer m_node;
 	size_type m_pos;

 private: 

 	friend class iterator_core_access;
    friend class cursor_core_access;
    
	friend class access_detach;
 	
    typename node_type::reference dereference() const
	{
		return helper<node_type, base_pointer, size_type>::deref(m_node, m_pos);
	}
	
    bool equal(cursor const& other) const
    {
        return (this->m_node == other.m_node) && (this->m_pos == other.m_pos);
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
		return m_node->operator[](m_pos)->empty();
	}
	
	size_type size_()
	{
		return m_node->size();
	}
	
	size_type max_size_()
	{
		return m_node->max_size();
	}
	
	size_type const par() const
	{
		return m_pos;
	}
	
public:

	cursor begin()
	{
		return cursor(m_node->operator[](m_pos), 0);
	}
	
	const_cursor left() const
	{
		return const_cursor(m_node->operator[](m_pos), 0);
	}

	cursor end()
	{
		return cursor(m_node->operator[](m_pos), m_node->size()-1);
	}

	const_cursor right() const
	{
		return const_cursor(m_node->operator[](m_pos), m_node->size()-1);
	}
	
	// Cursor stuff
	cursor parent()
	{
		return cursor(static_cast<base_pointer>(m_node->parent()), m_node->get_parity());
	}
	
	const_cursor up() const
	{
		return const_cursor(static_cast<base_pointer>(m_node->parent()), m_node->get_parity());
	}

public:	
	// TODO: protect?
	void attach(node_pointer p_node)
	{
		p_node->m_parent = m_node;
		
		// Only forest-relevant:
		p_node->operator[](1) = m_node->operator[](m_pos);
		m_node->operator[](m_pos)->m_parent = p_node;
		
		m_node->operator[](m_pos) = p_node;
	}

//	/** 
//	 * Detaches the node this cursor points to and returns a pointer to it;
//	 * this cursor will be set to its inorder successor afterwards (?)
//	 */
//	node_pointer detach()
//	{
//		return static_cast<node_pointer>(m_node->detach(m_pos));
//	}
//	
//	node_pointer detach(cursor y)
//	{
//		return static_cast<node_pointer>(m_node->detach(m_pos, y.m_pos, y.m_node));
//	}

};


} // namespace detail
} // namespace tree
} // namespace boost


#endif // BOOST_TREE_DETAIL_CURSOR_NARY_HPP
