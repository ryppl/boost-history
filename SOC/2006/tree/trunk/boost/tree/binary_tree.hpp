//  Copyright (c) 2006-2008, Bernhard Reiter
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
#include <boost/tree/algorithm/inorder.hpp>

#include <boost/tree/detail/node/traits.hpp>
#include <boost/tree/detail/cursor/nary.hpp>

#include <boost/test/minimal.hpp>

#include <memory>

namespace boost {
namespace tree {

using detail::node;
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
	typedef typename Alloc::template rebind<value_type>::other allocator_type;

 private:		
	typedef node<value_type, detail::binary_array> node_type;
	//typedef node<value_type, detail::binary_array> const_node_type;
	
	typedef typename Alloc::template rebind<node_type>::other 
		node_allocator_type;
	typedef typename node_traits<node_type>::node_base_type node_base_type;
	typedef node_base_type* node_base_pointer;
	typedef typename node_traits<node_type>::node_pointer node_pointer;
	
 public:
	typedef nary_tree_cursor<node_type> cursor;
	typedef /*const_*/ nary_tree_cursor<node_type const> const_cursor;

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
			: m_header(), m_value_alloc(alloc)
	{
		m_header[0] = node_base_type::nil();
		m_header[1] = &m_header;

		insert(root(), subtree);
	}
	
	/**
	 * @brief  Binary tree copy constructor.
	 * @param  x  A %binary_tree of identical element and allocator types.
	 * The newly-created %binary_tree uses a copy of the allocation object used
	 * by @a x.
	 */

	binary_tree (self_type const& x)
	: m_header(), m_value_alloc(x.m_value_alloc)
	{
		m_header[0] = node_base_type::nil();
		m_header[1] = &m_header;
		
		if (!x.empty())
			insert(root(), x.root());
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
		binary_tree temp(x);
		swap(temp);
		return *this;
	}
	
	template <class InputCursor>
		void assign(InputCursor subtree)
	{
		clear();
		insert(this->root(), subtree);
	}

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
		return const_cursor(/*cursor(*/&m_header, 0/*)*/);
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
	
	/**
	 * Returns true if the %binary_tree is empty.  (Thus root() would
	 * equal shoot().)
	 */
	bool empty() const
	{
		return m_header.m_parent == &m_header;
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
		subtree = subtree.begin();
		insert(pos, *subtree);
		if (!subtree.empty())
			insert(pos.begin(), subtree);
		if (!(++subtree).empty())
			insert(pos.end(), subtree);
		return pos.begin();
	}

//	//erase operations must rebalance; clear doesn't need to.	
//	//TODO: Can/Should remove (and erase) return the next cursor's position ?
//	//(There may be some DR concerning this for associative containers)
// 	void erase (cursor pos)
// 	{
// 		cursor ret; // = this->root();
//// 		pos = pos.parent();
//
//		// TODO: Get the following to work properly.
// 		//balancer_type::remove(*this, pos);
// 		node_pointer p_node;
//// 		if (pos == ret) {
//// 			augmentor_type::pre_detach(*this, pos);
// 			p_node = pos.detach();
//// 		} else {
//// 			augmentor_type::pre_detach(*this, pos, ret,);
//// 			p_node = pos.detach(ret);
//// 		}
// 		
//		m_value_alloc.destroy(p_node->data());
//		m_value_alloc.deallocate(p_node->data(), 1);
//		
//		m_node_alloc.destroy(p_node);
//		m_node_alloc.deallocate(p_node, 1);
//	}
 	
	 /** 
 	 * Removes a node and its descendants recursively in postorder
 	 * without rebalancing
 	 * @param c	Cursor pointing to the node to be removed.
 	 */
 	// TODO: Take care of header-pointers
 	void clear(cursor position) 
 	{
 		if (!position.empty()) {
 			node_pointer pos_node = 
 				static_cast<node_pointer>(position.m_node->operator[](position.m_pos));
	 		// delete the value position points to	
	 		m_value_alloc.destroy(pos_node->data());
	 		m_value_alloc.deallocate(pos_node->data(), 1);
	 		
			// recurse
	 		clear(position.begin());
	 		clear(position.end());
	 		
	 		// delete the node position points to
			m_node_alloc.destroy(pos_node);
			m_node_alloc.deallocate(pos_node, 1); 		
  		}
 	} 	
 	
	void rotate(cursor& pos)
	{
		//TODO: Take care of shoot/inorder_first pointers!
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
	 * @brief Clears all data from the tree (without any rebalancing).
	 */
 	void clear()
 	{
 		clear(this->root());
 		m_header.m_parent = &m_header;
 		m_header[0] = node_base_type::nil();
		m_header[1] = &m_header;
 	}
 	
	// splice operations
	
	//TODO: check for equal allocators...
	/**
	 * @brief  Insert contents of another %binary_tree.
	 * @param  position  Empty cursor that is going to be the root of the newly
	 *                   inserted subtree.
	 * @param  x  Source binary_tree.
	 * 
	 * The elements of @a x are inserted in constant time as the subtree whose
	 * root is referenced by @a position.  @a x becomes an empty
	 * binary_tree.
	 */
	void splice(cursor position, binary_tree& x)
	{
		if (!x.empty()) {
			if (position == shoot())         // Readjust shoot to x's
				m_header.m_parent = x.m_header.m_parent;
			if (position == inorder_first()) // Readjust inorder_first to x's
				m_header[1] = x.m_header[1];
				
			position.m_node->node_base_type::operator[](position.m_pos) = x.m_header[0];
			//TODO: replace the following by some temporary-swapping?
			x.m_header[0] = node_base_type::nil();
			x.m_header[1] = &x.m_header;
			x.m_header.m_parent = &x.m_header;
		}
	}
	
	// Does this splice *p or the subtree?
	// Maybe only *p, as the subtree would require knowledge about of shoot and
	// inorder_first in order to remain O(1)
	// But what if p has children?... Then this would require some defined
	// re-linking (splicing-out, actually) in turn!
//	void splice(cursor pos, binary_tree& x, cursor p)
//	{
//	}

	void splice(cursor position, binary_tree& x, 
				cursor root, cursor shoot, cursor inorder_first)
	{
		if (!x.empty()) {
			if (shoot == x.shoot())
				x.m_header.m_parent = root.m_node;
			if (inorder_first == x.inorder_first())
				x.m_header[1] = inorder_first.m_node;
			if (position == shoot())         // Readjust shoot to x's
				m_header.m_parent = shoot.m_node;
			if (position == inorder_first()) // Readjust inorder_first to x's
				m_header[1] = inorder_first.m_node;
				
			position.m_node->node_base_type::operator[](position.m_pos) = root.m_node;
			
			root.m_node[0] = node_base_type::nil();
			if (root == x.root()) {
				x.m_header[1] = &x.m_header;
				x.m_header.m_parent = &x.m_header;			
			} else
				root.m_node[1] = node_base_type::nil();
		}		
	}
	
	// TODO: only one inorder_erase?
	// Use boost::optional (or variant?) e.g.!!!
	// What we actually want is: return an object containing
	// all required (either one or two) parameters and that
	// is somehow applied to inorder_erase (either with one or two
	// parameters) -- but not requiring inorder_erase is provided
	// [from a "what should know what" POV.]
	// MPL?
	// The class that is supposed to handle this should probably have two ctors,
	// one taking one, the other taking two parameters
	/* 
	template <class T>
	class return_type_for_use_with_inorder_erase {
	public:
		ctor(T const& arg1): m_arg1(arg) {}
		ctor(T const& arg1, T const& arg2): m_arg1(arg1), m_arg2(arg2) {}
		
		// and, e.g.
		template <Fun F>
		call_function_with_arg_or_args(Fun) //... 
		//use like call_function_with_arg_or_args(mem_fun(inorder_erase))
		// will call inorder_erase(m_arg1) or inorder_erase(m_arg1, m_arg2), resp.
	private:
		// some object

	};
	*/

	// Nah. Guess a good compiler should just optimise that anyway.
	// That would still require a way of passing one or alternatively two values:
	// SO will it be (?): pair<cursor, optional<cursor> > //?

	/**
	 * @brief  Erases the value of the given cursor, keeping the binary_tree's
	 *         inorder, and returns the given cursor's inorder successor.
	 * @param  position  Cursor that is empty or at least one of whose children is
	 * @return The inorder successor of position
	 */
	cursor inorder_erase(cursor position)
	{
		node_pointer p_node = static_cast<node_pointer>(position.m_node);
		++position;
		
		if (position.empty()) {						
			(*p_node)[0]->m_parent = p_node->m_parent;
			static_cast<node_base_pointer>(p_node->m_parent)
				->node_base_type::operator[](0) = (*p_node)[0];
				
			while (position.parity()) 
				position = position.parent();
			if (position == root())
				position = shoot();
		} else {
			position = position.begin();
			position.m_node->m_parent = p_node->m_parent;
			static_cast<node_base_pointer>(p_node->m_parent)
				->node_base_type::operator[](1) = position.m_node;
			
			while (!position.empty())
				position = position.begin();
		}
		
		m_value_alloc.destroy(p_node->data());
		m_value_alloc.deallocate(p_node->data(), 1);
		
		m_node_alloc.destroy(p_node);
		m_node_alloc.deallocate(p_node, 1);
		
		return position;
	}

	cursor inorder_erase(cursor position, cursor descendant)
	{
		node_pointer p_node = static_cast<node_pointer>(descendant.m_node);
		++descendant;
		
		if (descendant.empty()) {						
			(*p_node)[0]->m_parent = p_node->m_parent;
			static_cast<node_base_pointer>(p_node->m_parent)
				->node_base_type::operator[](0) = (*p_node)[0];
		} else {
			descendant = descendant.begin();
			descendant.m_node->m_parent = p_node->m_parent;
			static_cast<node_base_pointer>(p_node->m_parent)
				->node_base_type::operator[](1) = descendant.m_node;
		}
		
		cursor pos_parent = position.parent();		
		pos_parent.m_node->node_base_type::operator[](pos_parent.m_pos)
			= descendant.m_node;
		descendant.m_node->m_parent = pos_parent.m_node;
		descendant.m_node->node_base_type::operator[](0)
			= position.m_node->node_base_type::operator[](0);
		descendant.m_node->node_base_type::operator[](1)
			= position.m_node->node_base_type::operator[](1);
		descendant.m_node->node_base_type::operator[](0)->m_parent
			= descendant.m_node;
		descendant.m_node->node_base_type::operator[](1)->m_parent
			= descendant.m_node;

		p_node = 
			static_cast<node_pointer>(position.m_node->node_base_type::operator[](position.m_pos));
			
		m_value_alloc.destroy(p_node->data());
		m_value_alloc.deallocate(p_node->data(), 1);
		
		m_node_alloc.destroy(p_node);
		m_node_alloc.deallocate(p_node, 1);
		
		return descendant;
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


namespace inorder {

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::first()) - O(1) overload for binary_tree
 * @param t	A binary_tree
 * @return	Mutable cursor to the first element of @a t in inorder traversal
 */
template <class T, class Alloc>
typename binary_tree<T, Alloc>::cursor 
first_cursor(binary_tree<T, Alloc>& t)
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
first_cursor(binary_tree<T, Alloc> const& t)
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
cfirst_cursor(binary_tree<T, Alloc> const& t)
{
	return t.inorder_first();
}

/**
 * @brief	First element of a binary_tree in inorder traversal
 * 			(equivalent to postorder::begin())
 * @param t	A binary_tree
 * @return	Mutable inorder iterator to the first element of @a t
 */
template <class T, class Alloc>
iterator<typename binary_tree<T, Alloc>::cursor>
begin(binary_tree<T, Alloc>& t)
{
	return iterator<typename binary_tree<T, Alloc>::cursor>(first_cursor(t));
}

/**
 * @brief	First element of a binary_tree in inorder traversal
 * 			(Alias of cbegin(); equivalent to postorder::begin())
 * @param t	A binary_tree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class T, class Alloc>
iterator<typename binary_tree<T, Alloc>::const_cursor>
begin(binary_tree<T, Alloc> const& t)
{
	return iterator<typename binary_tree<T, Alloc>::const_cursor>(first_cursor(t));
}

/**
 * @brief	First element of a binary_tree in inorder traversal
 * 			(equivalent to postorder::begin())
 * @param t	A binary_tree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class T, class Alloc>
iterator<typename binary_tree<T, Alloc>::const_cursor>
cbegin(binary_tree<T, Alloc> const& t)
{
//	typedef
//		typename cursor_vertical_traversal<
//			typename binary_tree<T, Alloc>::const_cursor
//		>::type traversal;
	return iterator<typename binary_tree<T, Alloc>::const_cursor>(first_cursor(t));
}


/**
 * @brief	One position past the last element of a binary_tree 
 * 			in inorder traversal
 * @param t	A binary_tree
 * @return	Mutable inorder iterator to the first element of @a t
 */
template <class T, class Alloc>
iterator<typename binary_tree<T, Alloc>::cursor>
end(binary_tree<T, Alloc>& t)
{
	return iterator<typename binary_tree<T, Alloc>::cursor>(last(t.root()));
}

/**
 * @brief	One position past the last element of a binary_tree
 * 			in inorder traversal. (Alias of cend().)
 * @param t	A binary_tree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class T, class Alloc>
iterator<typename binary_tree<T, Alloc>::const_cursor>
end(binary_tree<T, Alloc> const& t)
{
	return iterator<typename binary_tree<T, Alloc>::const_cursor>(last(t.root()));
}

/**
 * @brief	One position past the last element of a binary_tree
 * 			in inorder traversal
 * @param t	A binary_tree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class T, class Alloc>
iterator<typename binary_tree<T, Alloc>::const_cursor>
cend(binary_tree<T, Alloc> const& t)
{
	return iterator<typename binary_tree<T, Alloc>::const_cursor>(last(t.root()));
}

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BINARY_TREE_HPP
