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
 * @file binary_tree.hpp
 * Binary tree implementation
 */


#ifndef BOOST_TREE_BINARY_TREE_HPP
#define BOOST_TREE_BINARY_TREE_HPP

#include <boost/tree/cursor.hpp>
#include <boost/tree/iterators.hpp>

#include <boost/tree/detail/node/traits.hpp>
#include <boost/tree/detail/cursor/binary.hpp>
#include <boost/tree/detail/sortable_traits.hpp>

#include <boost/tree/augmentors/unaugmented.hpp>
#include <boost/tree/balancers/unbalanced.hpp>

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
template <class T, class Balance = balancers::unbalanced,
		  class Augment = augmentors::unaugmented,
		  class ValAlloc = std::allocator<T>, 
		  class NodeAlloc = ValAlloc // will be rebound.
		 >
class binary_tree : public Balance {
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


 public:
	binary_tree (value_allocator_type const& value_alloc = value_allocator_type(),
		  		 node_allocator_type const& node_alloc = node_allocator_type())
	: m_header(), m_value_alloc(value_alloc), m_node_alloc(node_alloc)
	{
		m_header.child[0] = node_base_type::nil();
		m_header.child[1] = &m_header;
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
		return cursor(&m_header, 0);
	}

	/**
	 * Returns a read-only const_cursor to the %binary_tree's root.
	 */ 	
	const_cursor root() const
	{
		return croot();
	}
	
	/**
	 * Returns a read-only const_cursor to the %binary_tree's root.
	 */ 	
	const_cursor croot() const
	{
		return const_cursor(&m_header, 0);
	}
	
	/**
	 * Returns a read/write ("mutable") cursor to the %binary_tree's shoot, i.e.
	 * one position past the last (inorder) value.
	 */ 	
	cursor shoot()
	{
		return cursor(m_header.m_parent, 
					  m_header.m_parent == &m_header ? 0 : 1);
	}
	
	/**
	 * Returns a read-only const_cursor to the %binary_tree's shoot, i.e. one
	 * position past the last (inorder) value.
	 */ 	
	const_cursor shoot() const
	{
		return cshoot;
	}
	
	/**
	 * Returns a read-only const_cursor to the %binary_tree's shoot, i.e. one
	 * position past the last (inorder) value.
	 */ 	
	const_cursor cshoot() const
	{
		return const_cursor(m_header.m_parent, 
							m_header.m_parent == &m_header ? 0 : 1);
	}
	
	/**
	 * Returns a read/write ("mutable") cursor to the first (inorder) value.
	 */ 	 
	cursor inorder_first()
	{
		return cursor(m_header.child[1], 0);
	}
	
	/**
	 * Returns a read-only const_cursor to the first (inorder) value.
	 */ 	 
	const_cursor inorder_first() const
	{
		return const_cursor(m_header.child[1], 0);
	}
	
	/// Functions returning (inorder) iterators (as required by the Sequence
	/// concept)
	
	/**
	 * Returns a read/write ("mutable") iterator to the first (inorder) value.
	 */ 	 
	iterator begin()
	{
		return iterator(inorder_first());
	}
	
	/**
	 * Returns a read-only const_iterator to the first (inorder) value.
	 */ 	 
	const_iterator begin() const
	{
		return cbegin();
	}
	
	/**
	 * Returns a read-only const_iterator to the first (inorder) value.
	 */ 	 
	const_iterator cbegin() const
	{
		return const_iterator(inorder_first());
	}

	/**
	 * Returns a read/write ("mutable") inorder iterator to the position one 
	 * past the last (inorder) value in the %binary_tree.  
	 */
	iterator end()
	{
		return iterator(shoot());
	}

	 /**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %binary_tree. 
	 */	
	const_iterator end() const
	{
		return cend();
	}
	
	/**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %binary_tree. 
	 */	
	const_iterator cend() const
	{
		return const_iterator(cshoot());
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
		p_node->init();
		
		pos.attach(p_node);

		// Readjust begin
		if ((iterator(pos) == this->begin()))
			m_header.child[1] = p_node; 
		
		// Readjust shoot
		if (pos == this->shoot())
			m_header.m_parent = p_node;

		//cursor c = this->root(); //TODO: revisit (add's type sig.)
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
	//TODO: Can/Should remove (and erase) return the next cursor's position ?
	//(There may be some DR concerning this for associative containers)
 	void erase (cursor pos)
 	{
 		cursor root = this->root();
 		pos = pos.parent();

 		balancer::remove(pos, root);
 		node_pointer p_node;
 		if (pos == root)
 			p_node = pos.detach();
		else
 			p_node = pos.detach(root);

		m_value_alloc.destroy(p_node->data());
		m_value_alloc.deallocate(p_node->data(), 1);
		
		m_node_alloc.destroy(p_node);
		m_node_alloc.deallocate(p_node, 1);
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
 		m_header.m_parent = &m_header;
 		m_header.child[0] = node_base_type::nil();
		m_header.child[1] = &m_header;
 	}

	bool empty() const
	{
		return m_header.m_parent == &m_header;
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
 		if (!c.empty()) {
 		
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
bool empty(tree_cursor<Node> cur)
{
	return !cur.empty();
}

template <class Node, class Balance, class ValAlloc, class NodeAlloc>
typename sortable_traits<binary_tree<Node, Balance, ValAlloc, NodeAlloc> >::container_type head(binary_tree<Node, Balance, ValAlloc, NodeAlloc>& t) //const.
{
	//return head_wrapper<Sortable>()(s);
	return t.root();
}

namespace inorder {

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::first()) - O(1) overload for binary_tree
 * @param t	A binary_tree
 * @return	Mutable cursor to the first element of @a t in inorder traversal
 */
template <class T, class Balance, class Augment, class ValAlloc, class NodeAlloc>
typename binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc>::cursor 
first(binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc>& t)
{
	return t.inorder_first();
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(Alias of cfirst(); equivalent to postorder::first()) -
 * 			O(1) overload for binary_tree
 * @param t	A binary_tree
 * @return	Read-only cursor to the first element of @a t in inorder traversal
 */
template <class T, class Balance, class Augment, class ValAlloc, class NodeAlloc>
typename binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc>::const_cursor 
first(binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc>& t)
{
	return t.inorder_first();
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::first()) - O(1) overload for binary_tree
 * @param t	A binary_tree
 * @return	Read-only cursor to the first element of @a t in inorder traversal
 */
template <class T, class Balance, class Augment, class ValAlloc, class NodeAlloc>
typename binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc>::const_cursor 
cfirst(binary_tree<T, Balance, Augment, ValAlloc, NodeAlloc>& t)
{
	return t.inorder_first();
}

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BINARY_TREE_HPP
