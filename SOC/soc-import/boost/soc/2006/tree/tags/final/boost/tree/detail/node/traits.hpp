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

// TODO: Move to detail/node/traits or so. reorganize.

#ifndef BOOST_TREE_DETAIL_NODE_TRAITS_HPP
#define BOOST_TREE_DETAIL_NODE_TRAITS_HPP


namespace boost {
namespace tree {


template <class Node>
struct node_traits
{
	typedef Node node_type;
	
	// Value
	typedef typename node_type::value_type	value_type;
	typedef typename node_type::pointer		pointer;
	typedef typename node_type::reference	reference;
	typedef typename node_type::size_type	size_type;
	
	// Node
	typedef typename node_type::node_pointer node_pointer;
	typedef typename node_type::node_reference node_reference;
	
	// Node base
	typedef typename node_type::base_type node_base_type;
	typedef typename node_type::base_pointer node_base_pointer;

	typedef typename node_type::augmentor augmentor;
	//typedef typename node_type::balancer balancer;
	
	typedef node_base_pointer position_type;
};

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_NODE_TRAITS_HPP
