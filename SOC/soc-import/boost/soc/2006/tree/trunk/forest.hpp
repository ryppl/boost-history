// Copyright 2006 Bernhard Reiter.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FOREST_HPP
#define FOREST_HPP

#include "tree.hpp"
#include "node_base_types.hpp"
#include "adaptors.hpp"
#include <memory>

# ifndef BOOST_NO_SFINAE
#  include <boost/type_traits/is_convertible.hpp>
#  include <boost/utility/enable_if.hpp>
# endif

/** \file forest.hh 
 * \brief Efficient, Flexible enough standard tree implementation 
 * This is a "hard-wired" standard (binary) tree implementation that offers
 * good time complexity even for a couple of otherwise expensive kinds of 
 * traversal, but with up to O(n) insertion time complexity, with n the number 
 * of nodes on a level.
 */

namespace
{
/**
 * In order to provide especially quick level and postorder traversal, this node
 * class uses pointers to its parent, previous and next sibling as well as its 
 * first child. The downside is that insertion of nodes has up to O(n) 
 * time complexity, with n the number of nodes on a level, as adjacent nodes'
 * sibling links have to be readjusted.
 */
// TODO: Concept check: T == Alloc::value_type
	template <typename T, typename Alloc>
	class node :
		public node_with_parent, 
		public node_with_first_child,
		public node_with_next_sibling, public node_with_previous_sibling 
	{
	public:
		typedef T value_type;
		typedef Alloc allocator_type;
		typedef node<value_type, allocator_type>* position_type;
		
		typedef boost::bidirectional_traversal_tag forest_horizontal_traversal;
		typedef node_with_parent parental_type;
	
		node(T const& value = T(), Alloc const& alloc = Alloc())
			: m_value(value), m_alloc(alloc) {}
		T& operator*() {return m_value;}
		
		// Container part. This is still a little flawy design-wise
		// should be moved altogether. for now, to sibling_iterator
		
		/** Must be static as we are using this method to create the root (and
		 * thus first in the tree) node -- otherwise we'd run into a chicken-egg
		 * problem. We should move this (insert) as a mem fun to parental type 
		 * -- which requires it to know about allocator... but what about 
		 * alloc<node>? do we need CRTP? is there another way?
		 * like parental_type<node> :
		 * 
		 */
		template <typename Pos>
		static void create_node(Pos& position, value_type const& value, 
			allocator_type& alloc, typename allocator_type::const_pointer hint = 0)
		{
			parent.first_child = alloc.allocate(1, hint);
			alloc.construct(parent.first_child, value);
		}
	
		/** Non-static version of the above, for invocation by iterator types;
		 * Aware of m_alloc. A little redundant, though.
		 */
		template <typename Parent>
		void insert_node(Parent& parent, value_type const& value, 
			typename allocator_type::const_pointer hint = 0)
		{
			create_node(parent, value, m_alloc, hint);
		}
		
		//destroy_child(
		
	 private:
		T m_value;
		allocator_type const& m_alloc; //Hm. This sucks a little, but I don't see much of 
		//a chance to get rid of it if we want parent_iter.insert(child_iter, value);
		//and the like...
	};

/* \class sibling_iter
 * Base template for sibling_iterator and const_sibling_iterator
 * with fallback declarations of increment() and decrement()
 * NOPE! I think we shouldn't provide fallback declarations for the more basic
 * traversal types as they have to make assumptions about the Node interface
 * (this leads to some kind of "interface redundancy" -- plus it makes finding
 * "collission-free" and reasonable naming of forest and binary tree difficult.
 * -- so these present sibling_iters should be moved to forest.hpp (as 
 * specialisations, probably also removing next_sibling() member functions and 
 * the like from (forest's) node. (make sibling_it and node friends)
 * (Or should we? These fns are kinda practical,
 * and it's not much more than increment() and decrement() that is affected...)
 */
// TODO: 
// what names should the different category/traversal types be given?
// forest_horizontal seems kinda ok, as it describes that
// we are checking for e.g. forward and backward iteration 
// "on the current horizontal level" -- we can use this from
// sibling_iterator, level_iterator, as well as from pre- and postorder
// iterators
// It's however a bit more difficult with other "directions",
// e.g. what is the best way to communicate if there is a parent() function?
// for a parent_iterator, and should we support e.g. a last_child()
// method from the template or just leave it to the specializations?
// Also note that I'm still a little unsure about how we should map
// forest-specific properties to binary ones in our node interface

// Can we remove second templ par safely, by replacing
// forest_horizontal_traversal in sibling_iter's template argument list
// with Node::forest_horzontal_traversal?

// TODO: what about a node_traits template?
//umm.. how do we partial specialize for a templated type (node<T>) ?
	template <typename Node, typename Alloc>
	class sibling_iter
	  : public boost::iterator_adaptor<
	        sibling_iter<Node, Alloc>
	      , typename Node::position_type
	      , typename Node::value_type
	      , typename Node::forest_horizontal_traversal
	    >
	{
	 private:
	    struct enabler {};
	    
	 public:
	 	typedef Alloc allocator_type;
	    sibling_iter(allocator_type const & alloc = allocator_type())
	      : sibling_iter::iterator_adaptor_(0), m_alloc(alloc) {}
	
	    explicit sibling_iter(Node* p, allocator_type const & alloc = allocator_type())
	      : sibling_iter::iterator_adaptor_(p), m_alloc(alloc) {}
	
	    template <class OtherNode>
	    sibling_iter(
	        sibling_iter<OtherNode, Alloc> const& other
	# ifndef BOOST_NO_SFINAE
	      , typename boost::enable_if<
	            boost::is_convertible<OtherNode*,Node*>
	          , enabler
	        >::type = enabler()
	# endif 
	    )
	      : sibling_iter::iterator_adaptor_(other.base()) {}
	
	//Container part:...
	//insert etc.
	//TODO: typename cleanup
	// ..and a lot more complicated logic...
	sibling_iter insert(sibling_iter pos, typename Node::value_type const& value)
	{
			
		//if (!this->empty()) 
		typename Node::position_type at = pos->base_reference()->previous_sibling;
		typename Node::position_type left = at->previous_sibling;
		
		at = m_alloc.allocate(1);
		m_alloc.construct(at, value);
		
		left->next_sibling = at;
		
		at->previous_sibling = left;
		at->parent = this->base();
		at->next_sibling = pos;
		
		pos->prev_sibling = at;
		
		return sibling_iter(at);
	}
	
	//how are we going to do the const counterparts?	
	sibling_iter begin()
	{
		return sibling_iter(this->base_reference()->first_child);
	}
		
	sibling_iter end()
	{
		return sibling_iter(this->base_reference()->last_child);
	}
	
	void push_back(typename Node::value_type const& value)
	{
		insert(this->end(), value);
	}
	
	void push_front(typename Node::value_type const& value)
	{

	}

	bool empty() { return begin() == end(); }
	
	# if !BOOST_WORKAROUND(__GNUC__, == 2)
	 private: // GCC2 can't grant friendship to template member functions    
	    friend class boost::iterator_core_access;
	# endif
		typename sibling_iter::iterator_adaptor_::reference dereference() const 
		{ return **(this->base_reference()); }
	    void increment() { this->base_reference() = this->base()->next_sibling(); }
	    void decrement() { this->base_reference() = this->base()->previous_sibling(); }
	
	    //container part
	    allocator_type const& m_alloc;
	    
	};

} // unnamed namespace

// Here go template specializations...
//

//Give us this day, our template typedefs.
//template<typename T, typename Alloc = std::allocator<T> > 
//	typedef tree<T, forest_navigation<T>, Alloc> forest;
template<typename T, typename Alloc = std::allocator<T> >
struct forest
{
	typedef tree<T, node<T, Alloc>, Alloc> type;
};

#endif // FOREST_HPP


