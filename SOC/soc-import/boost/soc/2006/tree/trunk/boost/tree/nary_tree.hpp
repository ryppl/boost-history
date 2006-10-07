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
 * @file nary_tree.hpp
 * Nary tree implementation
 */


#ifndef BOOST_TREE_NARY_TREE_HPP
#define BOOST_TREE_NARY_TREE_HPP

#include <boost/tree/cursor.hpp>
#include <boost/tree/iterators.hpp>

#include <boost/tree/detail/node/traits.hpp>
#include <boost/tree/detail/cursor/nary.hpp>
#include <boost/tree/detail/sortable_traits.hpp>

#include <boost/tree/augmentors/unaugmented.hpp>
#include <boost/tree/balancers/unbalanced.hpp>

#include <memory>
#include <vector>


namespace boost {
namespace tree {

using detail::node;
using detail::const_nary_tree_cursor;
using detail::nary_tree_cursor;

/** 
 * @brief An %nary_tree.
 * This class models the hierarchy concept, the container concept and the
 * sequence concept. TODO: complete this...
 *
 */

//We want: split/merge methods; will be reused by the multiway_tree adaptor.
//Are we going to have multiway_tree use balancers?
//Does nary_tree need them? Probably not, as nary_tree is functionally similar
//to forest... Though: we might need metadata...

template <class Tp, /*class Balance = balancers::unbalanced,
		  class Augment = augmentors::unaugmented,*/
		  class Alloc = std::allocator<Tp>, std::size_t N = /*numeric_limits<int>::max()*/ INT_MAX
		 >
class nary_tree /*: public Balance, public Augment*/ {
	typedef nary_tree<Tp, /*Balance, Augment,*/ Alloc, N> self_type;
 public:
	typedef Tp value_type;
	typedef balancers::unbalanced balancer_type;
	typedef augmentors::unaugmented augmentor_type;
	typedef typename Alloc::template rebind<value_type>::other 
		allocator_type;
	
	// We actually want a container whose capacity we can set and that doesn't
	// need an allocator...
	template <typename T>
	struct mycontainer //{
	: public std::vector<T, allocator_type> {
		explicit mycontainer(allocator_type const& = allocator_type())
		{}
		explicit mycontainer(typename std::vector<T, allocator_type>::size_type n, 
							 T const& = T(), 
							 allocator_type const& = allocator_type())
		{}
	};
	
	typedef node<value_type, mycontainer, typename augmentor_type::metadata_type, 
		typename balancer_type::metadata_type> node_type;
	typedef typename Alloc::template rebind<node_type>::other 
		node_allocator_type;
	typedef typename node_traits<node_type>::node_base_type node_base_type;
	typedef node_base_type* node_base_pointer;
	typedef typename node_traits<node_type>::node_pointer node_pointer;
	
	typedef nary_tree_cursor<node_type> cursor;
	typedef const_nary_tree_cursor<node_type> const_cursor;

//	typedef inorder::iterator<cursor> iterator;
//	typedef inorder::iterator<const_cursor> const_iterator;
	
//	typedef std::reverse_iterator<iterator> reverse_iterator;
//	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;
 public:
	explicit nary_tree (allocator_type const& alloc = allocator_type())
	: m_header(), m_value_alloc(alloc)
	{
		m_header.push_back(node_base_type::nil());
		m_header.push_back(&m_header);
		
//		m_header[0] = node_base_type::nil();
//		m_header[1] = &m_header;
	}

	explicit nary_tree (size_type n, value_type const& value = value_type(), 
		allocator_type const& alloc = allocator_type())
	: m_header(), m_value_alloc(alloc)
	{}

	template <class InputIterator>
		nary_tree (InputIterator first, InputIterator last, 
			allocator_type const& alloc = allocator_type())
			: m_value_alloc(alloc)
	{
		while (first++ != last)
			this->insert(this->end(), *first);
	}
	nary_tree (self_type const& other)
	: m_header(other.m_header), m_value_alloc(other.m_value_alloc)
	{}
	
	~nary_tree()
	{
		//clear();
	}
	
	nary_tree<Tp, /*Balance, Augment,*/ Alloc>& operator=(
		nary_tree<Tp, /*Balance, Augment,*/ Alloc> const& x);
	template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
	template <class Size, class T>
		void assign(Size n, const T& t = T());
	allocator_type get_allocator() const;
	
	/// Functions returning cursors (as required by the Hierarchy concept)
	
	/**
	 * Returns a read/write ("mutable") cursor to the %nary_tree's root.
	 */ 	
	cursor root()
	{
		return cursor(&m_header, 0);
	}

	/**
	 * Returns a read-only const_cursor to the %nary_tree's root.
	 */ 	
	const_cursor root() const
	{
		return croot();
	}
	
	/**
	 * Returns a read-only const_cursor to the %nary_tree's root.
	 */ 	
	const_cursor croot() const
	{
		return const_cursor(&m_header, 0);
	}
	
	/**
	 * Returns a read/write ("mutable") cursor to the %nary_tree's shoot, i.e.
	 * one position past the last (inorder) value.
	 */ 	
	cursor shoot()
	{
		return cursor(static_cast<node_base_pointer>(m_header.m_parent), 
					  m_header.m_parent == &m_header ? 0 : 1);
	}
	
	/**
	 * Returns a read-only const_cursor to the %nary_tree's shoot, i.e. one
	 * position past the last (inorder) value.
	 */ 	
	const_cursor shoot() const
	{
		return cshoot;
	}
	
	/**
	 * Returns a read-only const_cursor to the %nary_tree's shoot, i.e. one
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
		return const_cursor(m_header[1], 0);
	}
	
//	/// Functions returning (inorder) iterators (as required by the Sequence
//	/// concept)
//	
//	/**
//	 * Returns a read/write ("mutable") iterator to the first (inorder) value.
//	 */ 	 
//	iterator begin()
//	{
//		return iterator(inorder_first());
//	}
//	
//	/**
//	 * Returns a read-only const_iterator to the first (inorder) value.
//	 */ 	 
//	const_iterator begin() const
//	{
//		return cbegin();
//	}
//	
//	/**
//	 * Returns a read-only const_iterator to the first (inorder) value.
//	 */ 	 
//	const_iterator cbegin() const
//	{
//		return const_iterator(inorder_first());
//	}
//
//	/**
//	 * Returns a read/write ("mutable") inorder iterator to the position one 
//	 * past the last (inorder) value in the %nary_tree.  
//	 */
//	iterator end()
//	{
//		return iterator(shoot());
//	}
//
//	 /**
//	 * Returns a read-only inorder const_iterator to the position one past the 
//	 * last (inorder) value in the %nary_tree. 
//	 */	
//	const_iterator end() const
//	{
//		return cend();
//	}
//	
//	/**
//	 * Returns a read-only inorder const_iterator to the position one past the 
//	 * last (inorder) value in the %nary_tree. 
//	 */	
//	const_iterator cend() const
//	{
//		return const_iterator(cshoot());
//	}
	
	// Hierarchy-specific
	/**
	 * @brief		Inserts val in front of @a pos, or, if @a pos' parent is
	 * 				already full, creates a new child node containing @a val 
	 * 				instead.
	 * @param pos	The %nary_tree cursor in front of which to insert.
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

		p_node->m_parent = pos.m_parent;

		if (pos.m_parent == node_type::nil())
			static_cast<node_pointer>(pos.m_parent)->operator[](pos.m_pos) = p_node;
		else if (static_cast<node_pointer>(pos.m_parent)->empty()) {
			static_cast<node_pointer>(pos.m_parent)->push_back(p_node);
			pos.m_pos = 0;
		} else
			static_cast<node_pointer>(pos.m_parent)->insert(
				static_cast<node_pointer>(pos.m_parent)->begin()+(pos.m_pos), 
				p_node);
		return pos;
//		balancer_type::add(pos, this->root());
	}

	/// Sequence-specific
	
//	/**
//	 * @brief		Inserts val in front of @a pos, or, if @a pos' parent is
//	 * 				already full, creates a new child node containing @a val 
//	 * 				instead.
//	 * @param pos	The %nary_tree inorder iterator in front of which to insert.
//	 * @param val	The value to insert.
//	 * @return		An inorder iterator that points to the inserted data.
//	 */
//	iterator insert(iterator pos, value_type const& val)
//	{
//		return iterator(this->insert(cursor(pos), val));
//	}
	

 	
//	void erase (iterator pos)
// 	{		
// 		erase(cursor(pos));
// 	}
// 	
// 	void erase (iterator a, iterator b)
// 	{
// 	}

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

	bool empty() const
	{
		return m_header.m_parent == &m_header;
	}

private:

	node_base_type m_header;

	allocator_type	 m_value_alloc;
	node_allocator_type m_node_alloc;
	
	//erase operations must rebalance; clear doesn't need to.	
	//TODO: Can/Should remove (and erase) return the next cursor's position ?
	//(There may be some DR concerning this for associative containers)
 	void erase (cursor pos)
 	{
 		cursor root = this->root();
 		pos = pos.parent();

 		balancer_type::remove(pos, root);
 		node_pointer p_node;
 		if (pos == root) {
 			augmentor_type::pre_detach(pos, root);
 			p_node = pos.detach();
 		} else {
 			augmentor_type::pre_detach(pos, root, this->root());
 			p_node = pos.detach(root);
 		}
 		
		m_value_alloc.destroy(p_node->data());
		m_value_alloc.deallocate(p_node->data(), 1);
		
		m_node_alloc.destroy(p_node);
		m_node_alloc.deallocate(p_node, 1);
	}
};

//template <class Node, class Balance, class Alloc, class NodeAlloc>
//struct sortable_traits <class nary_tree<Node, Balance, Alloc, NodeAlloc> >
//{
//	typedef nary_tree<Node, Balance, Alloc, NodeAlloc> sortable_type;
//	typedef typename sortable_type::cursor cursor;
//	typedef typename sortable_type::value_type value_type;
// 	typedef typename sortable_type::size_type size_type;
// 	typedef cursor container_type;
//};
//
//template <class Node>
//bool empty(nary_tree_cursor<Node> cur)
//{
//	return !cur.empty();
//}
//
//template <class Node, class Balance, class Alloc, class NodeAlloc>
//typename sortable_traits<nary_tree<Node, Balance, Alloc, NodeAlloc> >::container_type head(nary_tree<Node, Balance, Alloc, NodeAlloc>& t) //const.
//{
//	//return head_wrapper<Sortable>()(s);
//	return t.root();
//}

//namespace inorder {
//
///**
// * @brief	First element of a MultiwayTree in inorder traversal
// * 			(equivalent to postorder::first()) - O(1) overload for nary_tree
// * @param t	A nary_tree
// * @return	Mutable cursor to the first element of @a t in inorder traversal
// */
//template <class T, class Balance, class Augment, class Alloc>
//typename nary_tree<T, Balance, Augment, Alloc>::cursor 
//first(nary_tree<T, Balance, Augment, Alloc>& t)
//{
//	return t.inorder_first();
//}
//
///**
// * @brief	First element of a MultiwayTree in inorder traversal
// * 			(Alias of cfirst(); equivalent to postorder::first()) -
// * 			O(1) overload for nary_tree
// * @param t	A nary_tree
// * @return	Read-only cursor to the first element of @a t in inorder traversal
// */
//template <class T, class Balance, class Augment, class Alloc>
//typename nary_tree<T, Balance, Augment, Alloc>::const_cursor 
//first(nary_tree<T, Balance, Augment, Alloc>& t)
//{
//	return t.inorder_first();
//}
//
///**
// * @brief	First element of a MultiwayTree in inorder traversal
// * 			(equivalent to postorder::first()) - O(1) overload for nary_tree
// * @param t	A nary_tree
// * @return	Read-only cursor to the first element of @a t in inorder traversal
// */
//template <class T, class Balance, class Augment, class Alloc>
//typename nary_tree<T, Balance, Augment, Alloc>::const_cursor 
//cfirst(nary_tree<T, Balance, Augment, Alloc>& t)
//{
//	return t.inorder_first();
//}
//
//} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_NARY_TREE_HPP
