// Copyright (c) 2006 Bernhard Reiter.

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file binary_tree.hpp
 * Binary tree implementation
 */

// TODO: Switch root/shoot (so begin() and inorder work properly?)

#ifndef BOOST_TREE_BINARY_TREE_HPP
#define BOOST_TREE_BINARY_TREE_HPP

#include <boost/tree/cursor.hpp>
#include <boost/tree/iterators.hpp>

#include <boost/tree/detail/node/traits.hpp>
#include <boost/tree/detail/cursor/binary.hpp>
#include <boost/tree/detail/sortable_traits.hpp>

#include <boost/tree/augmentors/trivial.hpp>
#include <boost/tree/balancers/trivial.hpp>

#include <boost/test/minimal.hpp>

#include <memory>

namespace boost {
namespace tree {

using detail::binary_node;
using detail::const_tree_cursor;
using detail::tree_cursor;


/** 
 * @brief A %binary_tree.
 * This class models the hierarchy concept, the container concept and the
 * sequence concept. TODO: complete this...
 *
*/
template <class T, class Balance = trivial_balance,
		  class Augment = trivial_augment,
		  class ValAlloc = std::allocator<T>, 
		  class NodeAlloc = // this exposes the node class...
		  	typename ValAlloc::template rebind<
			  	binary_node<T, Augment, typename Balance::metadata_type>
		  	>::other
		 >
class binary_tree {
	typedef binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc> self_type;
 public:
	typedef T value_type;
	typedef Balance balancer;
	typedef Augment augmentor;
	
	typedef binary_node<T, augmentor, typename balancer::metadata_type> node_type;
	
	typedef tree_cursor<node_type> cursor;
	typedef const_tree_cursor<node_type> const_cursor;

	typedef inorder::iterator<cursor> iterator;
	typedef inorder::iterator<const_cursor> const_iterator;
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	typedef typename cursor_pointer<cursor>::type pointer;
	typedef typename cursor_reference<cursor>::type reference;
	typedef typename cursor_size<cursor>::type size_type;
	typedef typename cursor_difference<cursor>::type difference_type;
	
	typedef typename node_traits<node_type>::node_base_type node_base_type;
	typedef typename node_traits<node_type>::node_pointer node_pointer;

	typedef typename ValAlloc::template rebind<value_type>::other 
		value_allocator_type;
	typedef typename NodeAlloc::template rebind<node_type>::other 
		node_allocator_type;

	
	binary_tree (value_allocator_type const& value_alloc = value_allocator_type(),
		  		 node_allocator_type const& node_alloc = node_allocator_type())
	: m_header(), m_value_alloc(value_alloc), m_node_alloc(node_alloc)
	{
		m_header.child[0] = &m_header;
		//m_header.child[1] = &m_header;
	}

	binary_tree (self_type const& other)
	: m_header(other.m_header), m_value_alloc(other.m_value_alloc), 
	  m_node_alloc(other.m_node_alloc) {}

	~binary_tree()
	{
		clear();
	}
	
	/// Functions returning cursors (as required by the Hierarchy concept)
	
	/**
	 * Returns a read/write ("mutable") cursor to the %binary_tree's root.
	 */ 	
	cursor root()
	{
		return cursor(&m_header, 1);
	}

	/**
	 * Returns a read-only const_cursor to the %binary_tree's root.
	 */ 	
	const_cursor root() const
	{
		//return const_cursor(m_header.m_parent);
		return const_cursor(&m_header, 1);
	}
	
	/**
	 * Returns a read/write ("mutable") cursor to the %binary_tree's shoot, i.e.
	 * one position past the last (inorder) value.
	 */ 	
	cursor shoot()
	{
		//return cursor(&m_header, 1);
		return cursor(m_header.m_parent, 1);
	}
	
	/**
	 * Returns a read-only const_cursor to the %binary_tree's shoot, i.e. one
	 * position past the last (inorder) value.
	 */ 	
	const_cursor shoot() const
	{
		//return const_cursor(&m_header, 1);
		return const_cursor(m_header.m_parent, 1);
	}
	
	/// Functions returning (inorder) iterators (as required by the Sequence
	/// concept)
	
	// FIXME: begin() is dysfunctional as of yet.
	// probably requires switching root/shoot and modification of
	// cursor::parent() to get it to work
	/**
	 * Returns a read/write ("mutable") iterator to the first (inorder) value.
	 */ 	 
	iterator begin()
	{
		if (root().has_child())
		//if (m_header.child[0] != m_header.m_parent)
			return iterator(cursor(m_header.child[0], 0));
		return iterator(cursor(&m_header, 1)); //iterator(shoot());
		
	}
	
	/**
	 * Returns a read-only const_iterator to the first (inorder) value.
	 */ 	 
	const_iterator begin() const
	{
		if (root().has_child())
		//if (m_header.child[0] != m_header.m_parent)
			return const_iterator(const_cursor(m_header.child[0], 0));
		//return const_iterator(const_cursor(&m_header, 1));
		return iterator(cursor(&m_header, 1)); //iterator(shoot());

	}

	/**
	 * Returns a read/write ("mutable") inorder iterator to the position one 
	 * past the last (inorder) value in the %binary_tree.  
	 */
	iterator end()
	{
		return iterator(shoot());
		//return iterator(cursor(m_header.m_parent, 1));
	}

	 /**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %binary_tree. 
	 */	
	const_iterator end() const
	{
		return const_iterator(shoot());
		//return const_iterator(const_cursor(m_header.m_parent, 1));
	}
	
	// Hierarchy-specific
	/**
	 * @brief		Inserts val in front of @a pos, or, if @a pos' parent is
	 * 				already full, creates a new child node containing @a val 
	 * 				instead.
	 * @param pos	The %binary_tree cursor in front of which to insert.
	 * @param val	The value to insert.
	 * @return		A cursor that points to the inserted data.
	 */
	cursor insert(cursor pos, value_type const& val)
	{
		//increment size!
		
		void* val_hint = 0;//TODO: need some method to obtain hints from cursor
		void* node_hint = 0;
		
		pointer p_val = m_value_alloc.allocate(1, val_hint);
		m_value_alloc.construct(p_val, val);
		
		node_pointer p_node = m_node_alloc.allocate(1, node_hint);
		m_node_alloc.construct(p_node, p_val);
		
		pos.add_node(p_node);
		// call balancer.

		// TODO: The following is really cumbersome. Find a better way.
		//if ((pos.m_parent == m_header.child[0]) && !pos.parity())
		//if ((iterator(pos.parent()) == this->begin()) && !pos.parity()) // && pos.parent() != root())
		//if ((iterator(pos) == this->begin()))
		if ((pos.m_parent == m_header.child[0]) && ((m_header.child[0] == &m_header) || !pos.parity()))
		
			m_header.child[0] = p_node; //pos.m_parent;	
		
		if (pos == this->shoot())
			m_header.m_parent = p_node;

//		if (iterator(pos) == this->begin())
//			m_header.child[0] = p_node;
	


		
		balancer::add(pos, this->root());
		return pos.begin(); 
	}

	/// Sequence-specific
	
	/**
	 * @brief		Inserts val in front of @a pos, or, if @a pos' parent is
	 * 				already full, creates a new child node containing @a val 
	 * 				instead.
	 * @param pos	The %binary_tree inorder iterator in front of which to insert.
	 * @param val	The value to insert.
	 * @return		An inorder iterator that points to the inserted data.
	 */
	iterator insert(iterator pos, value_type const& val)
	{
		return iterator(this->insert(cursor(pos), val));
	}
	
	//erase operations must rebalance; clear doesn't need to.	
 	void erase (cursor pos)
 	{		
 		//what if there are children?
 		//...splice out!
 		// if it's end(): delete value; 
 		//check if node is empty
 		// delete node.
 	}
 	
	void erase (iterator pos)
 	{		
		erase(cursor(pos));
 	}
 	
 	void erase (iterator a, iterator b)
 	{
 	}
 	
	/**
	 * @brief Clears all data from the tree (without any rebalancing).
	 */
 	void clear()
 	{
 		clear(this->root());
 	}


private:

	node_base_type m_header;

	value_allocator_type	 m_value_alloc;
	node_allocator_type m_node_alloc;
	
	 /** 
 	 * Removes a node and its descendants recursively in postorder
 	 * without rebalancing
 	 * @param c	Cursor pointing to the node to be removed.
 	 */
 	void clear(cursor c) 
 	{
 		if (has_child(c)) {
 		
	 		// delete the value this c points to	
	 		m_value_alloc.destroy(c.node()->data());
	 		m_value_alloc.deallocate(c.node()->data(), 1);
	 		
			// recurse
	 		clear(c.begin());
	 		clear(c.end());
	 		
	 		// delete the node c points to
			m_node_alloc.destroy(c.node());
			m_node_alloc.deallocate(c.node(), 1); 
		
  		}
 	}
};

template <class Node, class Balance, class ValAlloc, class NodeAlloc>
struct sortable_traits <class binary_tree<Node, Balance, ValAlloc, NodeAlloc> >
{
	typedef binary_tree<Node, Balance, ValAlloc, NodeAlloc> sortable_type;
	typedef typename sortable_type::cursor cursor;
	typedef typename sortable_type::value_type value_type;
 	typedef typename sortable_type::size_type size_type;
 	typedef cursor container_type;
};

template <class Node>
bool has_child(tree_cursor<Node> cur)
{
	return cur.has_child();
}

template <class Node, class Balance, class ValAlloc, class NodeAlloc>
typename sortable_traits<binary_tree<Node, Balance, ValAlloc, NodeAlloc> >::container_type head(binary_tree<Node, Balance, ValAlloc, NodeAlloc>& t) //const.
{
	//return head_wrapper<Sortable>()(s);
	return t.root();
}


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BINARY_TREE_HPP
