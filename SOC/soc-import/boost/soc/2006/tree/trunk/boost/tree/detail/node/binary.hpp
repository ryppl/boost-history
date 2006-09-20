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
 * @file binary.hpp
 * Binary node implementation
 */
 
//TODO: use Boost(TR1).Array (?)
//		and maybe Boost.Optional (for the "empty" stuff?)
//Templatize with arity (so we can use this for multiway trees, too?)

#ifndef BOOST_TREE_DETAIL_NODE_BINARY_HPP
#define BOOST_TREE_DETAIL_NODE_BINARY_HPP

#include <boost/array.hpp>

#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {

using boost::array;

//struct node_base;

template <std::size_t N>
class node_base : public array<node_base<N>*, N> {
	typedef node_base<N> self_type;
	
 public:
 
 	typedef array<node_base<N>*, N> base_type;
	typedef self_type* base_pointer;
	typedef self_type const* const_base_pointer;

	base_pointer m_parent;
	
	node_base()
	{
		m_parent = this;
	}
	
	static base_pointer nil()
	{
		static self_type m_nil_obj;
		static base_pointer m_nil = &m_nil_obj;
		return m_nil;
	}
	
	void init()
	{
		for (typename base_type::size_type i=0; i<base_type::max_size(); ++i)
			operator[](i) = nil();
	}
	
	base_pointer parent()
	{
		return m_parent;
	}
	
	base_pointer const parent() const
	{
		return m_parent;
	}
	
	// This injures Meyers' Item 36. OTOH, iterator adaptors do that, too, right?
	bool const empty() const
	{
		return (this == nil());
	}
	
	typename base_type::size_type rotate(typename base_type::size_type const& c)
	{
		//TODO: Optimise.
		//split up into even more atomic parts? probably not
		base_pointer q = base_type::operator[](c);
		
		base_pointer B = (base_type::operator[](c))->base_type::operator[](c ? 0 : 1);
		//pre_rotate();
		
		//B swaps places with its m_parent:

		base_type::operator[](c) = B;
		B->m_parent = this;
		q->m_parent = this->m_parent;

		typename base_type::size_type qp = get_parity();
		q->m_parent->base_type::operator[](qp) = q;
		this->m_parent = q;
		q->base_type::operator[](c ? 0 : 1) = this;
		return qp;
	}
	
	base_pointer detach(typename base_type::size_type m_pos)
	{
		base_pointer q = base_type::operator[](m_pos);
		base_type::operator[](m_pos) = 
			base_type::operator[](m_pos)
		  ->base_type::operator[]((base_type::operator[](m_pos))
		  ->base_type::operator[](0) == node_base::nil() ? 1 : 0);
		base_type::operator[](m_pos)->m_parent = this;
		return q;
	}
	
	// TODO: actually implement this.
	base_pointer detach(typename base_type::size_type parity, 
						typename base_type::size_type other_parity, 
						base_pointer other)
	{
		//Node::pre_splice(q, r);
		// splice out q
		base_pointer x = detach(parity);

		// q has been spliced out, now relink it in place of r.				
		other->m_parent->base_type::operator[](other_parity) = this;
		m_parent = other->m_parent;

		for (typename base_type::size_type i=0; i<base_type::max_size(); ++i) {
			base_type::operator[](i) = other->base_type::operator[](i);
			base_type::operator[](i)->m_parent = this;
		}
		return x;
	}
	
	typename base_type::size_type const get_parity() const
	{
		return (this->m_parent->base_type::operator[](0) == this ? 0 : 1);
	}
};

template <std::size_t N, typename T, class Augment, class BalanceData>
class node : public node_base<N> {
 public:
 	typedef T value_type;
	typedef Augment augmentor;
	typedef BalanceData balancer_data;

	typedef node<N, value_type, augmentor, balancer_data> node_type;
	typedef node_type* node_pointer;
	typedef node_type& node_reference;
	//typedef node_pointer position_type;
	typedef node_base<N> base_type;
	typedef base_type* base_pointer;
	typedef base_type const* const_base_pointer;
	
	typedef value_type& reference;
	typedef value_type* pointer;

	//enum size_t { first = 0, second = 1 };
	//typedef std::size_t size_type;
	
	struct metadata_type : public augmentor, public balancer_data {};
	
	// TODO: add observers.

	reference operator*() { return *m_data; } 
	
	node(pointer data) : base_type(), m_data(data) {}
	
	pointer data()
	{
		return m_data;
	}
	
	metadata_type& metadata()
	{
		return m_meta;
	}
	
 private:
	pointer m_data;
	metadata_type m_meta;
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_NODE_BINARY_HPP
