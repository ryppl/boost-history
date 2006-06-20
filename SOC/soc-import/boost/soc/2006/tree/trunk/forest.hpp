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
		
		typedef boost::bidirectional_traversal_tag forest_horizontal_traversal;
		typedef node_with_parent parental_type;
	
		node(T const& value = T(), Alloc const& alloc = Alloc())
			: m_value(value), m_alloc(alloc) {}
		T& operator*() {return m_value;}
		
		// Container part. This is still a little flawy design-wise
		
		/** Must be static as we are using this method to create the root (and
		 * thus first in the tree) node -- otherwise we'd run into a chicken-egg
		 * problem
		 */
		template <typename Parent>
		static void create_node(Parent& parent, value_type const& value, 
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
		
		// destroy_child
		
	 private:
		T m_value;
		allocator_type const& m_alloc; //Hm. This sucks a little, but I don't see much of 
		//a chance to get rid of it if we want parent_iter.insert(child_iter, value);
		//and the like...
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


