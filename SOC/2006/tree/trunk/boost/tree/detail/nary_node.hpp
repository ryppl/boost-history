//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file nary_node.hpp
 * Nary node implementation (with N=2, i.e. binary specialization)
 */
 
//Templatize with arity (so we can use this for multiway trees, too?)

#ifndef BOOST_TREE_DETAIL_NARY_NODE_HPP
#define BOOST_TREE_DETAIL_NARY_NODE_HPP

#include <boost/array.hpp>

#include <iterator>
#include <utility>

namespace boost {
namespace tree {
namespace detail {

using boost::array;

//template <class T> struct binary_array : public array<T, 2> { };

//struct node_base;
/*
 * node_with_parent_base
 */
 
class node_with_parent_base {
//    typedef node_with_parent_base self_type;
//    typedef self_type* base_pointer;
//    typedef self_type const* const_base_pointer;

public:
    node_with_parent_base* m_parent; // TODO: protect?
    
    node_with_parent_base()
    {
        m_parent = this;
    }
    
    node_with_parent_base(node_with_parent_base* p) : m_parent(p) {}
    
    node_with_parent_base* parent()
    {
        return m_parent;
    }
    
    node_with_parent_base* const parent() const
    {
        return m_parent;
    }
};

//template <template <typename> class Container>
//class node_base;
//
//template <template <typename> class Container>
//class node_base : public node_with_parent_base, public Container<node_base<Container>*> {
//    typedef node_base<Container> self_type;
//    
//public:
// 
//    typedef Container<node_base<Container>*> base_type;
//    typedef typename base_type::size_type size_type;
//    typedef self_type* base_pointer;
//    typedef self_type const* const_base_pointer;
//    
//    node_base() : node_with_parent_base()
//    { }
//    
//    static base_pointer nil()
//    {
//        static self_type m_nil_obj;
//        static base_pointer m_nil = &m_nil_obj;
//        return m_nil;
//    }
//    
//    void init()
//    {
//        for (typename base_type::size_type i=0; i<base_type::size(); ++i)
//            operator[](i) = nil();
//    }
//
//    // This injures Meyers' Item 36. OTOH, iterator adaptors do that, too, right?
//    bool const is_leaf() const
//    {
//        return ((this == nil()) || this->base_type::is_leaf());
//    }
//
//    // O(n); n is number of parent's children
//    typename base_type::size_type const get_index() const
//    {
//        typename base_type::size_type i = 0;
//        while (static_cast<base_pointer>(this->m_parent)->base_type::operator[](i++) != this);
//        return --i;
//        //return (static_cast<base_pointer>(this->m_parent)->base_type::operator[](0) == this ? 0 : 1);
//    }
//};

class node_base;

class node_with_children_base {
public:
    typedef array<node_base*, 2> children_type;
    
    node_with_children_base()
    {
        init();
    }

    inline void init()
    {
        for (children_type::size_type i=0; i<children_type::size(); ++i)
            m_children[i] = 0;
    }
//protected:
    children_type m_children;
};

class node_base
: public node_with_parent_base, public node_with_children_base {
    typedef node_base self_type;
    
public:
    //typedef node_with_children_base::base_type base_type;
    typedef self_type* base_pointer;
    typedef self_type const* const_base_pointer;
        
    node_base() : node_with_parent_base(), node_with_children_base()
    {
//        m_children[0] = 0;
//        m_children[1] = 0;
    }

    node_base(node_with_parent_base* p)
    : node_with_parent_base(p), node_with_children_base()
    {
//        m_children[0] = 0;
//        m_children[1] = 0;
    }
        
//    static base_pointer nil()
//    {
//        static self_type m_nil_obj;
//        static base_pointer m_nil = &m_nil_obj;
//        return m_nil;
//    }

    // This injures Meyers' Item 36. OTOH, iterator adaptors do that, too, right?
//    bool const is_leaf() const
//    {
//        return (this == nil());
//    }
    
    // Binary specific
    
    children_type::size_type rotate(children_type::size_type const& c)
    {
        //TODO: Optimise.
        base_pointer q = m_children[c];
        
        base_pointer B = m_children[c]->m_children[(c ? 0 : 1)];
        //pre_rotate();
        
        //B swaps places with its m_parent:

        m_children[c] = B;
        B->m_parent = this;
        q->m_parent = this->m_parent;

        children_type::size_type qp = get_index();
        static_cast<base_pointer>(q->m_parent)->m_children[qp] = q;
        this->m_parent = q;
        q->m_children[(c ? 0 : 1)] = this;
        return qp;
        //return (c ? 0 : 1);
    }

    void attach(base_pointer p_node, children_type::size_type m_pos)
    {
        p_node->m_parent = this;
        
        // Only relevant for non-leaf insertion:
        if (m_children[m_pos] != 0)
            m_children[m_pos]->m_parent = p_node;
        p_node->m_children[m_pos] = m_children[m_pos];

        m_children[m_pos] = p_node;
    }
    
    base_pointer detach(children_type::size_type m_pos)
    {
        base_pointer q = m_children[m_pos];
        m_children[m_pos] = 
            m_children[m_pos]
          ->m_children[((m_children[m_pos])
          ->m_children[0] == 0 ? 1 : 0)];
        m_children[m_pos]->m_parent = this;
        return q;
    }
    
    // TODO: actually implement this.
    base_pointer detach(children_type::size_type index, 
                        children_type::size_type other_index, 
                        base_pointer other)
    {
        //Node::pre_splice(q, r);
        // splice out q
        base_pointer x = detach(index);

        // q has been spliced out, now relink it in place of r.                
        static_cast<base_pointer>(other->m_parent)->m_children[other_index] = this;
        m_parent = other->m_parent;

        for (children_type::size_type i=0; i<children_type::max_size(); ++i) {
            m_children[i] = other->m_children[i];
            m_children[i]->m_parent = this;
        }
        return x;
    }
    
    // O(1)
    children_type::size_type const get_index() const
    {
        return (static_cast<base_pointer>(this->m_parent)->m_children[0] == this ? 0 : 1);
    }
    
};

class descending_node_base
: public node_with_children_base {
};

template <typename T>
class ascending_node : public node_base {
 public:
    typedef T value_type;

    typedef ascending_node<value_type> node_type;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;

    typedef node_base base_type;
    typedef base_type* base_pointer;
    typedef base_type const* const_base_pointer;
    
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* pointer;

    //enum size_t { first = 0, second = 1 };
    //typedef std::size_t size_type;

    reference operator*() { return m_data; } 

    const_reference operator*() const { return m_data; } 
    
    ascending_node(value_type data) : base_type(), m_data(data) {}
 
    ascending_node(value_type data, base_pointer p) : base_type(p), m_data(data) {}
    
private:
    value_type m_data;
};

} // namespace detail
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_NARY_NODE_HPP
