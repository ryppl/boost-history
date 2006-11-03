//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file balanced_tree.hpp
 * Balanced tree implementation
 */


#ifndef BOOST_TREE_BALANCED_TREE_HPP
#define BOOST_TREE_BALANCED_TREE_HPP

#include <boost/tree/cursor.hpp>
#include <boost/tree/iterators.hpp>

#include <boost/tree/detail/sortable_traits.hpp>

#include <boost/tree/balancers/unbalanced.hpp>

#include <boost/test/minimal.hpp>

#include <memory>

namespace boost {
namespace tree {

using detail::node;
using detail::const_nary_tree_cursor;
using detail::nary_tree_cursor;

/** 
 * @brief A %balanced_tree.
 * This class models the hierarchy concept, the container concept and the
 * sequence concept. TODO: complete this...
 *
*/
template <class Tp, class Hierarchy, class Balance>
class balanced_tree {
 public:
	typedef Tp value_type;
	typedef Balance balancer_type;
	typedef Hierarchy hierarchy_type;

 protected:
    hierarchy_type h;

 private:
	typedef balanced_tree<Tp, Hierarchy, Balance> self_type;
	
	typedef typename hierarchy_type::cursor cursor;
	typedef typename hierarchy_type::const_cursor const_cursor;

 public:	
	typedef inorder::iterator<cursor> iterator;
	typedef inorder::iterator<const_cursor> const_iterator;
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef typename hierarchy_type::allocator_type allocator_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;
 public:
	explicit balanced_tree (hierarchy_type const& hier = hierarchy_type())
	: h(hier)
	{}
	
	explicit balanced_tree (size_type n, value_type const& value = value_type(), 
		allocator_type const& alloc = allocator_type())
	: h(alloc)
	{}

	template <class InputIterator>
		balanced_tree (InputIterator first, InputIterator last, 
			allocator_type const& alloc = allocator_type())
			: h(alloc)
	{
		while (first++ != last)
			this->insert(this->end(), *first);
	}
	
	balanced_tree (self_type const& other)
	: h(other.h)
	{}
	
	self_type& operator=(
		self_type const& x);
	template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
	template <class Size, class T>
		void assign(Size n, const T& t = T());
	allocator_type get_allocator() const
	{
		return h.get_allocator();
	}
		
	/// Functions returning (inorder) iterators (as required by the Sequence
	/// concept)
	
	/**
	 * Returns a read/write ("mutable") iterator to the first (inorder) value.
	 */ 	 
	iterator begin()
	{
		return iterator(h.inorder_first());
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
		return const_iterator(h.inorder_cfirst());
	}

	/**
	 * Returns a read/write ("mutable") inorder iterator to the position one 
	 * past the last (inorder) value in the %balanced_tree.  
	 */
	iterator end()
	{
		return iterator(h.shoot());
	}

	 /**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %balanced_tree. 
	 */	
	const_iterator end() const
	{
		return cend();
	}
	
	/**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %balanced_tree. 
	 */	
	const_iterator cend() const
	{
		return const_iterator(h.cshoot());
	}
		
	/**
	 * @brief		Inserts val in front of @a pos, or, if @a pos' parent is
	 * 				already full, creates a new child node containing @a val 
	 * 				instead.
	 * @param pos	The %balanced_tree inorder iterator in front of which to insert.
	 * @param val	The value to insert.
	 * @return		An inorder iterator that points to the inserted data.
	 */
	iterator insert(iterator pos, value_type const& val)
	{
		cursor c = h.insert(cursor(pos), val);
		balancer_type::add(h, c);
		return iterator(c);
	}
	 	
	void erase (iterator pos)
 	{
 		h.erase(balancer_type::remove(*this, cursor(pos).parent()));
 	}
 	
 	void erase (iterator a, iterator b)
 	{
 	}
 	
	/**
	 * @brief Clears all data from the tree (without any rebalancing).
	 */
 	void clear()
 	{
 		h.clear();
 	}

	bool empty() const
	{
		return h.empty();
	}

};


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BALANCED_TREE_HPP
