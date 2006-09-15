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
 * Binary node implementation
 */
 
//TODO: use Boost(TR1).Array (?)
//		and maybe Boost.Optional (for the "empty" stuff?)
//Templatize with arity (so we can use this for multiway trees, too?)

#ifndef BOOST_TREE_DETAIL_NODE_BINARY_HPP
#define BOOST_TREE_DETAIL_NODE_BINARY_HPP


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
		m_parent = this;
	}
	
	// this has to be outsourced from the ctor - otherwise we'd be stuck in 
	// a recursion.
	void init()
	{
		child[0] = nil();
		child[1] = nil();
	}
	
	//TODO: copy ctor. this is not trivial - what about self referrring pointers,eg?

	static binary_node_base* nil()
	{
		static binary_node_base m_nil_obj;
		static binary_node_base* m_nil = &m_nil_obj;
		return m_nil;
	}
	
	base_pointer begin()
	{
		return child[0];
	}
	
	base_pointer end()
	{
		return child[1];
	}
	
	base_pointer parent()
	{
		return m_parent;
	}
	
	base_pointer const parent() const
	{
		return m_parent;
	}
	
	bool const empty() const
	{
		return (this == nil()); // && (this != this->m_parent));
		//return ((this != nil()) && (this != this->m_parent));
	}
	
	
	// The following adapted from the Austern et al. paper. Needs revisit bad
	// This should be wrapped by cursor!
	size_type rotate(size_type const& c)
	{
		//TODO: Optimise.
		//split up into even more atomic parts? probably not
		base_pointer q = this->child[c];
		
		base_pointer B = this->child[c]->child[c ? 0 : 1];
		//pre_rotate();
		
		//B swaps places with its m_parent:

		this->child[c] = B;
		B->m_parent = this;
		q->m_parent = this->m_parent;

		size_type qp = get_parity();
		q->m_parent->child[qp] = q;
		this->m_parent = q;
		q->child[c ? 0 : 1] = this;
		return qp;
	}
	
	base_pointer detach(size_type m_pos)
	{
		base_pointer q = child[m_pos];
		child[m_pos] = child[m_pos]->child[child[m_pos]->child[0] == 
										   binary_node_base::nil() ? 1 : 0];
		child[m_pos]->m_parent = this;
		return q;
	}
	
	// TODO: actually implement this.
	base_pointer detach(size_type parity, size_type other_parity, base_pointer other)
	{
		//Node::pre_splice(q, r);
		// splice out q
		base_pointer x = detach(parity);

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
	
	size_type const get_parity() const
	{
		return (this->m_parent->child[0] == this ? 0 : 1);
	}
};


template <typename T, class Augment, class BalanceData>
class binary_node : public binary_node_base {
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
	
	struct metadata_type : public augmentor, public balancer_data {};
	
	// TODO: add observers.

	pointer&		value() { return m_data; }
	reference	operator*() { return *m_data; } 
	
	binary_node(pointer data) : binary_node_base(), m_data(data)/*, m_size(1)*/  {}
		
	//move the following to node_base, then wrap around them?
//	size_type size()
//	{
//		return m_size;
//	}
	
	size_type max_size()
	{
		return 1;
	}

//	bool empty()
//	{
//		return m_size;
//	}

	//bool full?

	pointer data()
	{
		return m_data;
	}
	
	metadata_type& metadata()
	{
		return m_meta;
	}
	
	//call "do"rotate, splice (and pre_rotate, pre_splice) from rotate, splice.

 private:
	pointer m_data;
	metadata_type m_meta;
	//size_type m_size;
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_NODE_BINARY_HPP
