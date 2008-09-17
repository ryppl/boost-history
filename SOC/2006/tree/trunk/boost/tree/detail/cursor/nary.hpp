//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file nary.hpp
 * Nary cursor implementation
 */

#ifndef BOOST_TREE_DETAIL_CURSOR_NARY_HPP
#define BOOST_TREE_DETAIL_CURSOR_NARY_HPP


#include <boost/tree/cursor_facade.hpp>
#include <boost/tree/detail/node/nary.hpp>
#include <boost/tree/detail/iterator/inorder.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace boost {
namespace tree {
namespace detail {

using boost::iterator_core_access;
using boost::tree::cursor_core_access;

template <class Node> 
class nary_tree_cursor
 : public cursor_facade<nary_tree_cursor<Node>
      , typename mpl::eval_if<
                    is_const<Node>
                  , add_const<typename Node::value_type>
                  , mpl::identity<typename Node::value_type>
                >::type
      , random_access_traversal_tag
      , bidirectional_traversal_tag
    > {
 private:
      typedef typename Node::base_type node_base;
      
      typedef typename mpl::eval_if<
                        is_const<Node>
                      , add_const<node_base>
                      , mpl::identity<node_base>
                    >::type base_type;

    typedef base_type* base_pointer;
      
    struct enabler {};
     
     typedef Node node_type;
    typedef node_type* node_pointer;
    
public:

      typedef typename mpl::eval_if<
                        is_const<Node>
                       , add_const<typename Node::value_type>
                       , mpl::identity<typename Node::value_type>
                    >::type value_type;

    // Container-specific:
    typedef typename node_type::size_type size_type;

    // Cursor-specific
     typedef nary_tree_cursor<node_type> cursor;
     typedef nary_tree_cursor<node_type const> const_cursor;
    
    // Container-specific:
    typedef cursor iterator;
    typedef const_cursor const_iterator;
    
    template <class OtherValue>
    struct rebind {
        typedef nary_tree_cursor<OtherValue> other;
    };
    
    nary_tree_cursor()
      : m_node(0), m_pos(0) {}

    explicit nary_tree_cursor(base_pointer p, size_type pos) 
    : m_node(p), m_pos(pos) {}

    template <class OtherNode>
    nary_tree_cursor(
        nary_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, node_type*>
          , enabler
        >::type = enabler()
    )
      : m_node(other.m_node), m_pos(other.m_pos) {}

     base_pointer m_node;
     size_type m_pos;

 private: 

     friend class iterator_core_access;
    friend class cursor_core_access;
    
    friend class access_detach;
     
    value_type& dereference() const
    {
        return **static_cast<node_type*>(m_node);
    }
    
    bool equal(cursor const& other) const
    {
        return (this->m_node == other.m_node) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
        ++m_pos;
    }
    
    void decrement()
    {
        --m_pos;
    }    
    
    void advance(typename nary_tree_cursor::cursor_facade_::difference_type n)
    {
            m_pos += n;
    }
    
    typename nary_tree_cursor::cursor_facade_::difference_type
    distance_to(nary_tree_cursor z) const //FIXME: convertible to instead of nary_tree_cursor
    {
            return (z.m_pos - this->m_pos);
    }
    
    // Container specific
    bool empty_() const
    {
        return m_node->operator[](m_pos)->empty();
    }
    
    size_type size_() const
    {
        return m_node->size();
    }
    
    size_type max_size_() const
    {
        return m_node->max_size();
    }
    
    size_type par() const
    {
        return m_pos;
    }

    void left()
    {
        m_node = m_node->operator[](m_pos);
        m_pos  = 0;
    }

    void right()
    {
        size_type new_pos = m_node->size()-1; 
        m_node = m_node->operator[](m_pos);
        m_pos  = new_pos;
    }

    // Cursor stuff
    void up()
    {
        m_pos  = m_node->get_parity();
        m_node = static_cast<base_pointer>(m_node->parent());
    }
    
protected:
    bool is_on_top() const
    {
        base_pointer parent_begin_node = 
            static_cast<base_pointer>(m_node->parent())
            ->operator[](m_node->get_parity());
        return (!m_pos && (m_node != parent_begin_node));
        // (*this != this->parent().begin())
    }

public:
    // TODO: protect?
    void attach(node_pointer p_node)
    {
        p_node->m_parent = m_node;
        
        // Only forest-relevant:
        p_node->operator[](1) = m_node->operator[](m_pos);
        m_node->operator[](m_pos)->m_parent = p_node;
        
        m_node->operator[](m_pos) = p_node;
    }

//    /** 
//     * Detaches the node this cursor points to and returns a pointer to it;
//     * this cursor will be set to its inorder successor afterwards (?)
//     */
//    node_pointer detach()
//    {
//        return static_cast<node_pointer>(m_node->detach(m_pos));
//    }
//    
//    node_pointer detach(cursor y)
//    {
//        return static_cast<node_pointer>(m_node->detach(m_pos, y.m_pos, y.m_node));
//    }

};


} // namespace detail
} // namespace tree
} // namespace boost


#endif // BOOST_TREE_DETAIL_CURSOR_NARY_HPP
