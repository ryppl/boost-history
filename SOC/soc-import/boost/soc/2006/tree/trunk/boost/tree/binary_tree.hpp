//  Copyright (c) 2006, Bernhard Reiter
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
#include <boost/tree/detail/cursor/nary.hpp>
#include <boost/tree/detail/sortable_traits.hpp>

#include <boost/tree/augmentors/unaugmented.hpp>
#include <boost/tree/balancers/unbalanced.hpp>

#include <boost/test/minimal.hpp>

#include <memory>

namespace boost {
namespace tree {

using detail::node;
using detail::const_nary_tree_cursor;
using detail::nary_tree_cursor;

/** 
 * @brief A %binary_tree.
 * This class models the hierarchy concept, the container concept and the
 * sequence concept. TODO: complete this...
 *
*/
template < class Tp, class Alloc = std::allocator<Tp> >
class binary_tree {
	typedef binary_tree<Tp, Alloc> self_type;
 public:
	typedef Tp value_type;
	typedef typename Alloc::template rebind<value_type>::other 
		allocator_type;

 private:		
	typedef node<value_type, detail::binary_array> node_type;
	typedef typename Alloc::template rebind<node_type>::other 
		node_allocator_type;
	typedef typename node_traits<node_type>::node_base_type node_base_type;
	typedef node_base_type* node_base_pointer;
	typedef typename node_traits<node_type>::node_pointer node_pointer;
	
 public:
	typedef nary_tree_cursor<node_type> cursor;
	typedef const_nary_tree_cursor<node_type> const_cursor;

	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;
	
    template <class U> struct rebind {
      typedef binary_tree< U, typename allocator_type::template rebind<U>::other >
        other;
    };
    
	explicit binary_tree (allocator_type const& alloc = allocator_type())
	: m_header(), m_value_alloc(alloc)
	{
		m_header[0] = node_base_type::nil();
		m_header[1] = &m_header;
	}

	explicit binary_tree (size_type n, value_type const& value = value_type(), 
		allocator_type const& alloc = allocator_type())
	: m_header(), m_value_alloc(alloc)
	{}

	template <class InputCursor>
		binary_tree (InputCursor subtree,
			allocator_type const& alloc = allocator_type())
			: m_value_alloc(alloc)
	{
		insert(this->root(), subtree.root());
	}
	
	/**
	 * @brief  Binary tree copy constructor.
	 * @param  x  A %binary_tree of identical element and allocator types.
	 * The newly-created %binary_tree uses a copy of the allocation object used
	 * by @a x.
	 */
	binary_tree (self_type const& x)
	: m_header(x.m_header), m_value_alloc(x.m_value_alloc)
	{
		insert(this->root(), x.root());
		
		// TODO. The following lines take extra care of the header. A better 
		// template <InputCursor> insert(...) function might make them obsolete
		if (x.m_header[0] == &(x.m_header))
			m_header[0] = &m_header;
		if (x.m_header[1] == &(x.m_header))
			m_header[1] = &m_header;
		if (x.m_header.m_parent == &(x.m_header))
			m_header.m_parent = &m_header;
	}
	
	~binary_tree()
	{
		clear();
	}
	
	/**
	 * @brief  Binary tree assignment operator.
	 * @param  x  A %binary_tree of identical element and allocator types.
	 * 
	 * All the elements of @a x are copied, but unlike the
	 * copy constructor, the allocator object is not copied.
	 */
	binary_tree<Tp, Alloc>& operator=(binary_tree<Tp, Alloc> const& x)
	{
		self_type temp(x);
		swap(temp);
		return *this;
	}
	
	template <class InputCursor>
		void assign(InputCursor subtree);

    /// Get a copy of the memory allocation object.
	allocator_type get_allocator() const
	{
		return m_value_alloc;
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
		return cursor(static_cast<node_base_pointer>(m_header.m_parent), 
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
		return const_cursor(static_cast<node_base_pointer>(m_header.m_parent), 
							m_header.m_parent == &m_header ? 0 : 1);
	}
	
	/**
	 * Returns a read/write ("mutable") cursor to the first (inorder) value.
	 */ 	 
	cursor inorder_first()
	{
		return cursor(m_header[1], 0);
	}
	
	/**
	 * Returns a read-only const_cursor to the first (inorder) value.
	 */ 	 
	const_cursor inorder_first() const
	{
		return inorder_cfirst();
	}

	/**
	 * Returns a read-only const_cursor to the first (inorder) value.
	 */ 	 
	const_cursor inorder_cfirst() const
	{
		return const_cursor(m_header[1], 0);
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
		if ((pos == this->inorder_first()))
			m_header[1] = p_node; 
		
		// Readjust shoot
		if (pos == this->shoot())
			m_header.m_parent = p_node;

		//cursor c = this->root(); //TODO: revisit (add's type sig.)
		//balancer_type::add(*this, pos);
		
		return pos.begin(); 
	}

	// TODO: Optimise. Especially wrt header links. 
	// Making the other insert() a special case of this one might be more
	// reasonable.
	/**
	 * @brief		Inserts val in front of @a pos, or, if @a pos' parent is
	 * 				already full, creates a new child node containing @a val 
	 * 				instead.
	 * @param pos	The %binary_tree inorder iterator in front of which to insert.
	 * @param val	The value to insert.
	 * @return		An inorder iterator that points to the inserted data.
	 */
    template <class InputCursor>
	cursor insert(cursor pos, InputCursor subtree)
	{
		while (!subtree.empty()) {
			insert(pos, *subtree);
			insert(pos.begin(), subtree.begin());
			insert(pos.end(), subtree.end());
		}
		return pos;
	}

	//erase operations must rebalance; clear doesn't need to.	
	//TODO: Can/Should remove (and erase) return the next cursor's position ?
	//(There may be some DR concerning this for associative containers)
 	void erase (cursor pos)
 	{
 		cursor ret; // = this->root();
// 		pos = pos.parent();

		// TODO: Get the following to work properly.
 		//balancer_type::remove(*this, pos);
 		node_pointer p_node;
// 		if (pos == ret) {
// 			augmentor_type::pre_detach(*this, pos);
 			p_node = pos.detach();
// 		} else {
// 			augmentor_type::pre_detach(*this, pos, ret,);
// 			p_node = pos.detach(ret);
// 		}
 		
		m_value_alloc.destroy(p_node->data());
		m_value_alloc.deallocate(p_node->data(), 1);
		
		m_node_alloc.destroy(p_node);
		m_node_alloc.deallocate(p_node, 1);
	}
 	
	 /** 
 	 * Removes a node and its descendants recursively in postorder
 	 * without rebalancing
 	 * @param c	Cursor pointing to the node to be removed.
 	 */
 	// TODO: Take care of header-pointers
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
 	
	/**
	 * @brief Clears all data from the tree (without any rebalancing).
	 */
 	void clear()
 	{
 		clear(this->root());
 		m_header.m_parent = &m_header;
 		m_header[0] = node_base_type::nil();
		m_header[1] = &m_header;
 	}

	void rotate(cursor& pos)
	{
		pos.m_pos = pos.m_node->rotate(pos.m_pos);
		pos.m_node = static_cast<node_base_pointer>(pos.m_node->m_parent->m_parent);
	}
	
	/**
	 * @brief  Swaps data with another %binary_tree.
	 * @param  x  A %binary_tree of the same element and allocator types.
	 * 
	 * This exchanges the elements between two binary trees in constant time.
	 * (Four pointers, so it should be quite fast.)
	 */
	void swap(self_type& other)
	{
		using std::swap;
		if (empty()) {
			if (other.empty())
				return;
				
			m_header[0] = other.m_header[0];
			m_header[0]->m_parent = &m_header;

			m_header[1] = other.m_header[1];
			m_header.m_parent = other.m_header.m_parent;
			
			other.m_header[0] = node_base_type::nil();
			other.m_header[1] = &other.m_header;
			other.m_header.m_parent = &other.m_header;
			
			return;
		}
		
		if (other.empty()) {
			other.m_header[0] = m_header[0];
			other.m_header[0]->m_parent = &other.m_header;
			
			other.m_header[1] = m_header[1];
			other.m_header.m_parent = m_header.m_parent;
			
			m_header[0] = node_base_type::nil();
			m_header[1] = &m_header;
			m_header.m_parent = &m_header;
			
			return;			
		}
		
		swap(m_header, other.m_header);
		//swap(m_header[0]->m_parent, other.m_header[0]->m_parent);
		m_header[0]->m_parent = &m_header;
		other.m_header[0]->m_parent = &other.m_header;
		
		return;
	}

	/**
	 * Returns true if the %binary_tree is empty.  (Thus root() would
	 * equal shoot().)
	 */
	bool empty() const
	{
		return m_header.m_parent == &m_header;
	}

private:

	node_base_type m_header;

	allocator_type	 m_value_alloc;
	node_allocator_type m_node_alloc;
	

};

/// See boost::tree::binary_tree::swap().
template <class T, class Alloc>
inline void swap(binary_tree<T, Alloc>& x, binary_tree<T, Alloc>& y)
{
	x.swap(y);
}

template <class T, class Alloc>
struct sortable_traits <class binary_tree<T, Alloc> >
{
	typedef binary_tree<T, Alloc> sortable_type;
	typedef typename sortable_type::cursor cursor;
	typedef typename sortable_type::value_type value_type;
 	typedef typename sortable_type::size_type size_type;
 	typedef cursor container_type;
};

template <class Node>
bool empty(nary_tree_cursor<Node> cur)
{
	return !cur.empty();
}

template <class T, class Alloc>
typename sortable_traits<binary_tree<T, Alloc> >::container_type head(binary_tree<T, Alloc>& t) //const.
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
template <class T, class Alloc>
typename binary_tree<T, Alloc>::cursor 
first(binary_tree<T, Alloc>& t)
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
template <class T, class Alloc>
typename binary_tree<T, Alloc>::const_cursor 
first(binary_tree<T, Alloc>& t)
{
	return t.inorder_first();
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::first()) - O(1) overload for binary_tree
 * @param t	A binary_tree
 * @return	Read-only cursor to the first element of @a t in inorder traversal
 */
template <class T, class Alloc>
typename binary_tree<T, Alloc>::const_cursor 
cfirst(binary_tree<T, Alloc>& t)
{
	return t.inorder_first();
}

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BINARY_TREE_HPP
