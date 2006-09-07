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
 * Binary cursor template specialization
 */

// TODO: Use TR1 type_traits (integral_constant)
// can we abstract the cursor stuff any further, 
// eventually producing cursor_adaptor?

#ifndef BOOST_TREE_DETAIL_CURSOR_BINARY_HPP
#define BOOST_TREE_DETAIL_CURSOR_BINARY_HPP

#include <boost/tree/detail/node/binary.hpp>
#include <boost/tree/detail/cursor/general.hpp>

#include <boost/tree/augmentors/trivial.hpp>
#include <boost/tree/balancers/trivial.hpp>
#include <boost/tree/binary_tree.hpp>

#include <boost/type_traits/integral_constant.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {


template<class T, class Augment, class Balance> 
class const_tree_cursor<binary_node<T, Augment, Balance> > 
 : public boost::iterator_facade<const_tree_cursor<binary_node<T, Augment, Balance> >
      , T const //const is a hint for iterator_facade!
      , random_access_traversal_tag
    > {
 private:
    struct enabler {};

 public:
 	//TODO: Tidy up typedefs
 	 	
 	typedef binary_node<T, Augment, Balance> const node_type;
	typedef node_type* node_pointer;

	typedef typename binary_node<T, Augment, Balance>::base_pointer 
		base_pointer;	
	typedef typename binary_node<T, Augment, Balance>::const_base_pointer 
		const_base_pointer;
	
	// Container-specific:
	typedef typename node_type::size_type size_type;

	// Cursor-specific
	typedef integral_constant<size_type, 2> arity; // binary cursor
		
 	typedef tree_cursor<binary_node<T, Augment, Balance> > cursor;
 	typedef const_tree_cursor<binary_node<T, Augment, Balance> > const_cursor;

	typedef bidirectional_traversal_tag vertical_traversal_type;
	
	typedef typename node_type::metadata_type metadata_type;
	
	// Container-specific:
	typedef cursor iterator;  // For (range) concepts' sake, mainly
	typedef const_cursor const_iterator;
	
 	// Common iterator facade stuff
    const_tree_cursor()
     : m_parent(0), m_pos(0) {}

    explicit const_tree_cursor(
		const_base_pointer p, 
		size_type pos /*= 1*/)
     : m_parent(p), m_pos(pos) {}
      
    template <class OtherNode>
    const_tree_cursor(
        const_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, 
           	typename binary_node<T, Augment, Balance>::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : m_parent(other.m_parent), m_pos(other.m_pos) {}

	
 private:
	const_base_pointer m_parent;
 	size_type m_pos;

    friend class boost::iterator_core_access;
    
    T const& dereference() const
	{
		return **static_cast<node_pointer>(m_parent);
	}
	
    bool equal(const_tree_cursor const& other) const
    {
		return (this->m_parent == other.m_parent) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
		++m_pos;
    }
    
    void decrement()
    {
		--m_pos;
    }
    
    void advance(typename iterator_facade<const_tree_cursor, const T, 
					random_access_traversal_tag, const T&, 
					int>::difference_type n)
    {
    		m_pos += n;
    }
    
    typename iterator_facade<const_tree_cursor, const T, 
    	random_access_traversal_tag, const T&, int>::difference_type
    distance_to(const_tree_cursor z) const //TODO: convertible to instead of const_tree_cursor (?)
    {
    		return (z.m_pos - this->m_pos);
    }
	
public:
	// Container-specific
	size_type const size() const
	{
		return 1;
	}
	
	size_type const max_size() const
	{
		return 1;
	}
	
	// TODO (following couple of functions): wrap around node member fn
	const_cursor begin() const
	{
		return const_cursor(m_parent->child[m_pos], 0); 
	}
		
	const_cursor end() const
	{
		return const_cursor(m_parent->child[m_pos], 1);
	}
	
	// Cursor stuff. 
	
	const_cursor parent() const
	{
		return const_cursor(m_parent->parent(), m_parent->get_parity());
	}
		
	size_type const parity() const
	{
		return m_pos;
	}
		
	bool const empty() const
	{
		return m_parent->child[m_pos]->empty();
	}
	
//	node_pointer node() // really? certainly not public
//	{
//		return static_cast<node_pointer>(m_parent->child[m_pos]);
//	}
	
	metadata_type const& metadata() const
	{
		return static_cast<node_pointer>(m_parent->child[m_pos])->metadata();
	}
};

template<class T, class Augment, class Balance> 
class tree_cursor<binary_node<T, Augment, Balance> > 
 : public boost::iterator_facade<tree_cursor<binary_node<T, Augment, Balance> >
      , T
      , random_access_traversal_tag
    > {
 private:
  	typedef typename binary_node<T, Augment, Balance>::base_pointer base_pointer;
    struct enabler {};

 public:
 	typedef binary_node<T, Augment, Balance> node_type;
	typedef node_type* node_pointer;

	// Container-specific:
	typedef typename node_type::size_type size_type;

	// Cursor-specific
	typedef integral_constant<size_type, 2> arity; // binary cursor
		
 	typedef tree_cursor<binary_node<T, Augment, Balance> > cursor;
 	typedef const_tree_cursor<binary_node<T, Augment, Balance> > const_cursor;

	typedef bidirectional_traversal_tag vertical_traversal_type;
 
 	typedef typename node_type::metadata_type metadata_type;
 	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;


    tree_cursor()
      : m_parent(0), m_pos(0) {}

    explicit tree_cursor(base_pointer p, size_type pos /*= 1*/)
      : m_parent(p), m_pos(pos) {}

    template <class OtherNode> //revisit
    tree_cursor(
        tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, base_pointer>
          , enabler
        >::type = enabler()
    )
      : m_parent(other.m_parent), m_pos(other.m_pos) {}

 private: 
 	base_pointer m_parent;
 	size_type m_pos;

 	friend class boost::iterator_core_access;
 	
    T& dereference() const
	{
		return **static_cast<node_pointer>(m_parent);
	}
	
    bool equal(cursor const& other) const
    {
        return (this->m_parent == other.m_parent) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
		++m_pos;
    }
    
    void decrement()
    {
		--m_pos;
    }    
    
	void advance(typename iterator_facade<tree_cursor, const T, 
					random_access_traversal_tag, const T&, 
					int>::difference_type n)
    {
    		m_pos += n;
    }
    
    typename iterator_facade<tree_cursor, const T, 
    		random_access_traversal_tag, const T&, int>::difference_type
    distance_to(tree_cursor z) const //FIXME: convertible to instead of const_tree_cursor
    {
    		return (z.m_pos - this->m_pos);
    }
	
public:
	// Container specific
	size_type size()
	{
		return 1;
	}
	
	size_type max_size()
	{
		return 1;
	}
	
	cursor begin()
	{
		return cursor(m_parent->child[m_pos], 0);
	}
	
	const_cursor begin() const
	{
		return const_cursor(m_parent->child[m_pos], 0); //make a node.begin()
	}
		
	cursor end()
	{
		return cursor(m_parent->child[m_pos], 1);
	}

	const_cursor end() const
	{
		return const_cursor(m_parent->child[m_pos], 1);
	}
	
	// Cursor stuff
	cursor parent()
	{
		return cursor(m_parent->parent(), m_parent->get_parity());
	}
	
	const_cursor parent() const
	{
		return const_cursor(m_parent->parent(), m_parent->get_parity());
	}
	
	node_pointer node() 
	{
		return static_cast<node_pointer>(m_parent->child[m_pos]);
	}
	
	size_type const parity() const
	{
		return m_pos;
	}
		
	bool const empty() const
	{
		return m_parent->child[m_pos]->empty();
	}
	
	bool const is_root() const
	{
		return ((m_parent->m_parent->child[0] != m_parent) 
			 && (m_parent->m_parent->child[1] != m_parent))
			 ||  m_parent->m_parent == m_parent->child[1]; // empty root
	}
	
	void rotate()
	{
		m_pos = m_parent->rotate(m_pos);
		m_parent = m_parent->m_parent->m_parent;
	}
	
	void add_node(node_pointer p_node)
	{
		p_node->m_parent = m_parent;
		m_parent->child[m_pos] = p_node;
	}
	
	metadata_type const& metadata() const
	{
		return static_cast<node_pointer>(m_parent->child[m_pos])->metadata();
	}
	
	metadata_type& metadata()
	{
		return const_cast<metadata_type&>
			  (static_cast<cursor const*>(this)->metadata());
	}
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_CURSOR_BINARY_HPP
