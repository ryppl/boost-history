// Copyright 2006 Bernhard Reiter.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TREE_HH
#define TREE_HH

#include "adaptors.hpp"

//	Design rationale related:
//	The tree constructor must create root (using allocator)
//	which returns a pointer to root which we must then use to
//	obtain a  positional object.
//	How do we achieve this?
//	1. the "fertile soil" approach: create a node instance (the "soil"), add 
//	the root as its child; downside: overhead, esp. for simple node types
//	2. conditional/specialised constructor: root is of type allocator::pointer
//	we could have explicit constructors for our positional objects that take
//	this type and act correspondingly (how? in/decrement operators won't work
//	as before if we can't convert to position_base_type -- no real solution?)
//	3. add a soil type to navigation with child members only ? (tree ctor:
//	construct soil (using allocator), ...
//	For implementational 
//	independency, we can only guarantee to get a positional object if root itself
//	is another node's child, or, for reduced overhead, a child of an object of 
//	parental_type which in turn is the part of class node that is responsible
//	for storing children. 
//	So we'd like 3, but for simplicity, we'll stick to 1 for the moment.
	

/** @brief A generic %tree container.
 * 
 * This is a skeleton type providing general properties of a tree.
 * Specific traversal and other access functions are retrieved through
 * iterators built around Node.
 *
*/
//TODO: Concept Checks
//T == Alloc::value_type
//T == Node::value_type
template <typename T, typename Node, typename Alloc = std::allocator<Node> >
class tree 
{
 public:
	typedef T value_type;
	typedef Node node_type;
	typedef Alloc allocator_type;

	// TODO: inactivate if corresponding traversal tags are missing
	// via enable_if? (need to look into MPL/concepts)
	typedef sibling_iter<node_type, allocator_type> sibling_iterator;
	typedef sibling_iter<node_type const, allocator_type> sibling_const_iterator;
	//other iterators...

//	tree (T const & value = T(), Alloc const & alloc = Alloc()) 
//		: m_alloc(alloc), m_soil(node_type::parental_type())
//	{
//		node_type::create_child(root(), value, m_alloc);
//	}
	
//	~tree()
//	{
//	}
	
	sibling_iterator root()
	{
		//return sibling_iterator(m_soil.first_child());
	}
	
private:
	allocator_type m_alloc;
	typename node_type::parental_type m_soil;	
};

#endif // TREE_HPP
