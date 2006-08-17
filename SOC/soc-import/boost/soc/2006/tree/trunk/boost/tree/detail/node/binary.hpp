// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file binary_tree.hpp
 * Binary node implementation
 */
 
//TODO: use Boost(TR1).Array 
//		and maybe Boost.Optional (for the "has_child" stuff?)
//Templatize with arity (so we can use this for multiway trees, too?)

#ifndef BOOST_TREE_DETAIL_NODE_BINARY_HPP
#define BOOST_TREE_DETAIL_NODE_BINARY_HPP


#include <boost/tree/augmentors/trivial.hpp>
#include <boost/tree/balancers/trivial.hpp>

#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {

struct binary_node_base //TODO: make this a class (friend of binary_tree?)
{
	typedef binary_node_base* base_pointer;
	typedef binary_node_base const* const_base_pointer;
	base_pointer m_parent;
	base_pointer child[2];
	
	typedef std::size_t size_type; //TODO: use cheaper type. we only need 0, 1 !
	
	binary_node_base()
	{
		child[0] = nil;
		child[1] = nil;
		m_parent = this;
	}
	
	//TODO: copy ctor. this is not trivial - what about self referrring pointers,eg?

	static binary_node_base nil_obj;
	static binary_node_base* nil;
	
	base_pointer begin()
	{
		return child[0];
	}
	
	base_pointer end()
	{
		return child[1];
	}
	
	base_pointer& parent()
	{
		return m_parent;
	}
	
	// The following adapted from the Austern et al. paper. Needs revisit bad
	// This should be wrapped by cursor!
	void rotate() // TODO: get parity parameter
	{
		//TODO: Optimise.
		//split up into even more atomic parts? probably not
		size_type c = (m_parent->child[0] == this ? 0 : 1);
		base_pointer p = m_parent;
		
		base_pointer B = child[c ? 0 : 1];
		
		//pre_rotate;
		
		//B swaps places with its m_parent:
		p->child[c] = B;
		B->m_parent = p;
		//B->parity = m_pos;
		this->m_parent = p->m_parent;
		size_type qp = (this->m_parent->child[0] == this ? 0 : 1);
		
		this->m_parent->child[qp] = this;
		p->m_parent = this;
		this->child[c ? 0 : 1] = p;
	}
	
	
	base_pointer splice_out(size_type parity)
	{
		//Node::pre_splice(q);
		base_pointer x = child[child[0] == binary_node_base::nil];
		x->m_parent = m_parent;
		m_parent->child[parity] = x;
		return x;
	}
	
	base_pointer splice_out(size_type parity, size_type other_parity, base_pointer other)
	{
		//Node::pre_splice(q, r);
		// splice out q
		base_pointer x = child[child[0] == binary_node_base::nil];
		x->m_parent = m_parent;
		m_parent->child[parity] = x;

		// q has been spliced out, now relink it in place of r.				
		other->m_parent->child[other_parity] = this;
		m_parent = other->m_parent;
		//q->parity = r->parity;
		child[0] = other->child[0];
		child[1] = other->child[1];
		child[0]->m_parent = this;
		child[1]->m_parent = this;
		return x;
	}
	
};

binary_node_base binary_node_base::nil_obj;
binary_node_base* binary_node_base::nil = &binary_node_base::nil_obj;


template <typename T, class Augment = trivial_augment, class BalanceData = trivial_metadata>
class binary_node
: public binary_node_base, public Augment, public BalanceData {
 public:
 	typedef T value_type;
	typedef Augment augmentor;
	typedef BalanceData balancer_data;

	typedef binary_node<value_type, augmentor, balancer_data> node_type;
	typedef node_type* node_pointer;
	typedef node_type& node_reference;
	//typedef node_pointer position_type;
	typedef binary_node_base base_type;
	typedef base_type* base_pointer;
	typedef base_type const* const_base_pointer;
	
	typedef value_type& reference;
	typedef value_type* pointer;

	 //enum size_t { first = 0, second = 1 };
	typedef std::size_t size_type;
	
	// TODO: add observers.

	pointer&		value() { return m_data; }
	reference	operator*() { return *m_data; } 
	
	binary_node(pointer data) : binary_node_base(), m_data(data), m_size(1)  {}
		
	//move the following to node_base, then wrap around them?
	size_type size()
	{
		return m_size;
	}
	
	size_type max_size()
	{
		return 1;
	}

	bool empty()
	{
		return m_size;
	}

	//bool full?

	pointer data()
	{
		return m_data;
	}
		
	//call "do"rotate, splice (and pre_rotate, pre_splice) from rotate, splice.

 private:
	pointer m_data;
	size_type m_size;
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_NODE_BINARY_HPP
