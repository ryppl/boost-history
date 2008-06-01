//  Copyright (c) 2006-2008, Bernhard Reiter
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
	//typedef typename node_type::size_type	size_type;
	
	// Node
	typedef typename node_type::node_pointer node_pointer;
	typedef typename node_type::node_reference node_reference;
	
	// Node base
	typedef typename node_type::base_type node_base_type;
	typedef typename node_type::base_pointer node_base_pointer;
	
	typedef node_base_pointer position_type;
};

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_NODE_TRAITS_HPP
