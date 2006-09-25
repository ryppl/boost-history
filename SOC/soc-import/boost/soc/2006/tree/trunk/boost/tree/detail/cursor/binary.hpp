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
 * Binary cursor template
 */

// TODO: Use TR1 type_traits (integral_constant)
// can we abstract the cursor stuff any further, 
// eventually producing cursor_adaptor?

#ifndef BOOST_TREE_DETAIL_CURSOR_BINARY_HPP
#define BOOST_TREE_DETAIL_CURSOR_BINARY_HPP

#include <boost/tree/detail/node/binary.hpp>

#include <boost/type_traits/integral_constant.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/iterator/iterator_facade.hpp>


#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {

using boost::iterator_core_access;

class access_rotate {
 public:
      template <class Cursor>
      static void rotate(Cursor& c)
      {
          c.rotate();
      }
};

class cursor_core_access {
 public:
  	template <class Facade>
	static bool empty_(Facade const& f)
	{
		return f.empty_();
	}
	
 	template <class Facade>
	static typename Facade::size_type size_(Facade const& f)
	{
		return f.size_();
	}
	
	template <class Facade>
	static typename Facade::size_type max_size_(Facade const& f)
	{
		return f.max_size_();
	}
	
	template <class Facade>
	static Facade left(Facade const& f)
	{
		return f.left();
	}
	
	template <class Facade>
	static Facade right(Facade const& f)
	{
		return f.right();
	}
	
	template <class Facade>
	static Facade up(Facade const& f)
	{
		return f.up();
	}
};

template <class Hor, class Vert, class T, class Dist = std::ptrdiff_t,
		  class Size = std::size_t, class Ptr = T*, class Ref = T&>
class cursor {
	typedef Hor		horizontal_iterator_traversal;
	typedef Vert		vertical_iterator_traversal;
	
	typedef Size		size_type;
	
	typedef Hor		iterator_category;
	typedef T		value_type;
	typedef Dist		difference_type;
	typedef Ptr		pointer;
	typedef Ref		reference;
};

template <
    class Derived             // The derived cursor type being constructed
  , class Value
  , class HorizontalCategoryOrTraversal
  , class VerticalCategoryOrTraversal
  , class Reference   = Value&
  , class Difference  = std::ptrdiff_t
  , class Size		  = std::size_t
>
class cursor_facade 
 : public iterator_facade<Derived, Value, HorizontalCategoryOrTraversal, 
						  Reference, Difference> {
 private:
 	//typedef Derive<Value> Derived;
	//
	// Curiously Recurring Template interface.
	//
	Derived& derived()
	{
	    return *static_cast<Derived*>(this);
	}
	
	Derived const& derived() const
	{
	    return *static_cast<Derived const*>(this);
	}

//	typedef iterator_facade<Derived, Value, HorizontalCategoryOrTraversal, 
//							Reference, Difference> iterator_facade_;
	typedef typename cursor_facade::iterator_facade_ iterator_facade_;
 protected:
 	// For use by derived classes
	typedef cursor_facade<Derived, Value, HorizontalCategoryOrTraversal,
						  VerticalCategoryOrTraversal, Reference, Difference> 
			cursor_facade_;
 public:

	typedef typename iterator_facade_::value_type value_type;
	typedef Reference reference;
	typedef Difference difference_type;
	typedef typename iterator_facade_::pointer pointer;
	typedef typename iterator_facade_::iterator_category iterator_category;

	typedef Size size_type;

	typedef bidirectional_traversal_tag vertical_traversal_type;

//	typedef typename Derived::template rebind<value_type>::other cursor;
//	typedef typename Derived::template rebind<add_const<value_type> >::other
//			const_cursor;

	bool const empty() const
	{
		return cursor_core_access::empty_(this->derived());
	}
	
	size_type const size() const
	{
		return cursor_core_access::size_(this->derived());
	}
	
	size_type const max_size() const
	{
		return cursor_core_access::max_size_(this->derived());
	}
		 
 	// if Value is const: Derived == const_cursor: only Derived
 	// otherwise: also Derived const. Really? implicit conversion to const_cursor?
 	Derived begin()
 	{
		return cursor_core_access::left(this->derived()); 		
 	}

 	Derived end()
 	{
		return cursor_core_access::right(this->derived()); 		
 	}
 	
 	Derived parent()
 	{
		return cursor_core_access::up(this->derived()); 		
 	}
 	//if Value isn't const: also cursor.
};

template <
    class Derived
  , class Base
  , class Value               = use_default
  , class HorizontalTraversal = use_default
  , class VerticalTraversal	  = use_default
  , class Reference           = use_default
  , class Difference          = use_default
  , class Size                = use_default
>
class cursor_adaptor 
  : public iterator_adaptor<Derived, Base, Value, HorizontalTraversal, Reference,
  							Difference>
{
	friend class iterator_core_access;
	friend class cursor_core_access;
	typedef iterator_adaptor<Derived, Base, Value, HorizontalTraversal, Reference,
  							Difference> iterator_adaptor_;
 public:
    cursor_adaptor() : iterator_adaptor_()
    { }
    
    explicit cursor_adaptor(Base const& iter) : iterator_adaptor_(iter)
    { }
    
    typedef HorizontalTraversal horizontal_traversal;
    typedef HorizontalTraversal vertical_traversal;
    
    typedef Size size_type;
    typedef Base base_type;
    
    
 protected:
    typedef cursor_adaptor cursor_adaptor_;

 public:
 	bool const empty_() const
	{
		return iterator_adaptor_::base().empty();
	}
	
	size_type const size_() const
	{
		return iterator_adaptor_::base().size();
	}
	
	size_type const max_size_() const
	{
		return iterator_adaptor_::base().max_size();
	}
	
	Derived begin()
	{
		return Derived(this->base_reference().begin());
	}
		
	Derived up()
	{
		return Derived(this->base_reference().parent());
	}
};


template <class Node>
class tree_cursor;

template<class Node>
class const_tree_cursor
 : public cursor_facade<const_tree_cursor<Node>
      , typename Node::value_type const //const is a hint for iterator_facade!
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
    struct enabler {};
	typedef const_tree_cursor<Node> self_type;
	typedef typename self_type::cursor_facade_ cursor_facade_;
 public:
 	//TODO: Tidy up typedefs
 	 	
 	typedef Node const node_type;
	typedef node_type* node_pointer;

	typedef typename Node::base_pointer base_pointer;	
	typedef typename Node::const_base_pointer const_base_pointer;
	
	// Container-specific:
	typedef typename cursor_facade_::size_type size_type;

	// Cursor-specific
	typedef integral_constant<size_type, 2> arity; // binary cursor
	
 	typedef tree_cursor<Node> cursor;
 	typedef const_tree_cursor<Node> const_cursor;

	//typedef bidirectional_traversal_tag vertical_traversal_type;
	
	typedef typename Node::metadata_type metadata_type;
	
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
           	typename Node::base_pointer>  // is that correct?
          , enabler
        >::type = enabler()
    )
      : m_parent(other.m_parent), m_pos(other.m_pos) {}

	
 private:
	const_base_pointer m_parent;
 	size_type m_pos;

    friend class iterator_core_access;
    friend class cursor_core_access;
        
    typename Node::value_type const& dereference() const
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
    
    void advance(typename const_tree_cursor::cursor_facade_::difference_type n)
    {
    		m_pos += n;
    }
    
    typename const_tree_cursor::cursor_facade_::difference_type
    distance_to(const_tree_cursor z) const //TODO: convertible to instead of const_tree_cursor (?)
    {
    		return (z.m_pos - this->m_pos);
    }
	
private:
	// Container-specific
	
	bool const empty_() const
	{
		return m_parent->operator[](m_pos)->empty();
	}
	
	size_type const size_() const
	{
		return m_parent->size();
	}
	
	size_type const max_size_() const
	{
		return m_parent->max_size();
	}
	
	// TODO (following couple of functions): wrap around node member fn
	const_cursor left() const
	{
		return const_cursor(m_parent->operator[](m_pos), 0); 
	}
		
	const_cursor right() const
	{
		return const_cursor(m_parent->operator[](m_pos), m_parent->size()-1);
	}

	// Cursor stuff. 	
	const_cursor up() const
	{
		return const_cursor(m_parent->parent(), m_parent->get_parity());
	}

public:

		
	size_type const parity() const
	{
		return m_pos;
	}

	metadata_type const& metadata() const
	{
		return static_cast<node_pointer>(m_parent->operator[](m_pos))->metadata();
	}
};

template <class Node> 
class tree_cursor//
 : public cursor_facade<tree_cursor<Node>
      , typename Node::value_type
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
  	typedef typename Node::base_pointer base_pointer;
    struct enabler {};

 public:
 	typedef Node node_type;
	typedef node_type* node_pointer;

	// Container-specific:
	typedef typename node_type::size_type size_type;

	// Cursor-specific
	typedef integral_constant<size_type, 2> arity; // binary cursor
		
 	typedef tree_cursor<node_type > cursor;
 	typedef const_tree_cursor<node_type> const_cursor;

	typedef bidirectional_traversal_tag vertical_traversal_type;
 
 	typedef typename node_type::metadata_type metadata_type;
 	
	// Container-specific:
	typedef cursor iterator;
	typedef const_cursor const_iterator;
	
	template <class OtherValue>
	struct rebind {
		typedef tree_cursor<OtherValue> other;
	};
	
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

 	friend class iterator_core_access;
    friend class cursor_core_access;
    
 	friend class access_rotate;
 	friend class access_detach;
 	
    typename node_type::reference dereference() const
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
    
	void advance(typename tree_cursor::cursor_facade_::difference_type n)
    {
    		m_pos += n;
    }
    
    typename tree_cursor::cursor_facade_::difference_type
    distance_to(tree_cursor z) const //FIXME: convertible to instead of const_tree_cursor
    {
    		return (z.m_pos - this->m_pos);
    }
    
	// Container specific
	bool const empty_() const
	{
		return m_parent->operator[](m_pos)->empty();
	}
	
	size_type size_()
	{
		return m_parent->size();
	}
	
	size_type max_size_()
	{
		return m_parent->max_size();
	}	
public:

	
	cursor begin()
	{
		return cursor(m_parent->operator[](m_pos), 0);
	}
	
	const_cursor left() const
	{
		return const_cursor(m_parent->operator[](m_pos), 0);
	}

	cursor end()
	{
		return cursor(m_parent->operator[](m_pos), m_parent->size()-1);
	}

	const_cursor right() const
	{
		return const_cursor(m_parent->operator[](m_pos), m_parent->size()-1);
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
		return static_cast<node_pointer>(m_parent->operator[](m_pos));
	}
	
	size_type const parity() const
	{
		return m_pos;
	}
		

	
//	bool const is_root() const
//	{
//		return ((m_parent->m_parent->operator[](0) != m_parent) 
//			 && (m_parent->m_parent->operator[](1) != m_parent))
//			 ||  m_parent->m_parent == m_parent->operator[](1); // empty root
//	}

protected:	
	void rotate()
	{
		m_pos = m_parent->rotate(m_pos);
		m_parent = m_parent->m_parent->m_parent;
	}
	

public:	
	// TODO: protect?
	void attach(node_pointer p_node)
	{
		p_node->m_parent = m_parent;
		
		// Only forest-relevant:
		p_node->operator[](1) = m_parent->operator[](m_pos);
		m_parent->operator[](m_pos)->m_parent = p_node;
		
		m_parent->operator[](m_pos) = p_node;
	}

	/** 
	 * Detaches the node this cursor points to and returns a pointer to it;
	 * this cursor will be set to its inorder successor afterwards (?)
	 */
	node_pointer detach()
	{
		return static_cast<node_pointer>(m_parent->detach(m_pos));
	}
	
	node_pointer detach(cursor y)
	{
		return static_cast<node_pointer>(m_parent->detach(m_pos, y.m_pos, y.m_parent));
	}
		
	metadata_type const& metadata() const
	{
		return static_cast<node_pointer>(m_parent->operator[](m_pos))->metadata();
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
