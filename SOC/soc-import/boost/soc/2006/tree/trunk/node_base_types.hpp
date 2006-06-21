// Copyright 2006 Bernhard Reiter.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** \file node_byse_types.hh 
 * \brief Trivial helper classes for node composition.
 * TODO Probably rename some
 * Still unclear how to best map forest - binary correspondence.
 */
 
#ifndef NODE_BASE_TYPES_HH
#define NODE_BASE_TYPES_HH

struct node_with_parent {
 private:
	node_with_parent() : m_parent(0) {}
	node_with_parent* parent() 
	{
		return m_parent;
	}
 private:
	node_with_parent* m_parent;
};

struct node_with_next_sibling {
	node_with_next_sibling() : m_next_sibling(0) {}
	node_with_next_sibling* next_sibling() 
	{
		return m_next_sibling;
	}
 private:
	node_with_next_sibling* m_next_sibling;
};


struct node_with_previous_sibling {
	node_with_previous_sibling() : m_previous_sibling(0) {}
	node_with_previous_sibling* previous_sibling() 
	{
		return m_previous_sibling;
	}
 private:
	node_with_previous_sibling* m_previous_sibling;
};

class node_with_first_child {
public:
	node_with_first_child() : m_first_child(0) {}
	node_with_first_child* first_child() 
	{
		return m_first_child;
	}
	
 private:
	node_with_first_child* m_first_child;
};

#endif // NODE_BASE_TYPES_HH
