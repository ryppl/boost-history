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
 
class node_with_parent_base {
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
    
    void attach(node_with_parent_base* p_node)
    {
        p_node->m_parent = this;
    }
};

class node_with_children_base;

class node_with_children_base {
public:
    typedef array<node_with_children_base*, 2> children_type;
    
    node_with_children_base()
    {
        init();
    }

    inline void init()
    {
        for (children_type::size_type i=0; i<children_type::size(); ++i)
            m_children[i] = 0;
    }

    void attach(node_with_children_base* p_node, children_type::size_type m_pos)
    {
        // Only relevant for non-leaf insertion:
        p_node->m_children[m_pos] = m_children[m_pos];

        m_children[m_pos] = p_node;
    }

    void splice(node_with_children_base* p_node, children_type::size_type m_pos)
    {
        m_children[m_pos] = p_node->m_children[m_pos];

        p_node->m_children[m_pos] = 0; 
    }

//protected:
    children_type m_children;
};

class node_base
: public node_with_parent_base, public node_with_children_base {
    typedef node_base self_type;
    
public:
    typedef self_type* base_pointer;
    typedef self_type const* const_base_pointer;
        
    node_base() : node_with_parent_base(), node_with_children_base() {}

    node_base(node_with_parent_base* p)
    : node_with_parent_base(p), node_with_children_base() {}
    
    // Binary specific
    
    children_type::size_type rotate(children_type::size_type const& c)
    {
        //TODO: Optimise.
        base_pointer q = static_cast<node_base*>(m_children[c]);
        
        base_pointer B = static_cast<node_base*>(m_children[c]->m_children[(c ? 0 : 1)]);
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
        node_with_parent_base::attach(p_node);
        
        // Only relevant for non-leaf insertion:
        if (m_children[m_pos] != 0)
            static_cast<node_base*>(m_children[m_pos])->m_parent = p_node;

        node_with_children_base::attach(p_node, m_pos);
    }

    void splice(base_pointer p_node, children_type::size_type m_pos)
    {
        static_cast<node_base*>(p_node->m_children[m_pos])->m_parent = this;

        node_with_children_base::splice(p_node, m_pos);
    }
    
    base_pointer detach(children_type::size_type m_pos)
    {
        node_with_children_base::children_type::size_type parent_idx = get_index();

        if (m_children[m_pos] != 0) // Set child's parent only if there _is_ a child
            static_cast<node_base*>(m_children[m_pos])->m_parent = m_parent;

        static_cast<node_base*>(m_parent)->m_children[parent_idx]
            = m_children[m_pos];
        
        return &*this;
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
