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

#include <boost/tree/cursor_adaptor.hpp>
#include <boost/tree/detail/node/nary.hpp>

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
 : public cursor_adaptor<nary_tree_cursor<Node>
      , typename mpl::eval_if<
                        is_const<Node>
                      , add_const<typename Node::base_type>
                      , mpl::identity<typename Node::base_type>
                    >::type*                  
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
    typedef typename nary_tree_cursor::cursor_adaptor_::size_type size_type;

    // Cursor-specific
    typedef nary_tree_cursor<node_type> cursor;
    typedef nary_tree_cursor<node_type const> const_cursor;
    typedef std::size_t subtree_size_type;
    
    // Container-specific:
    typedef cursor iterator;
    typedef const_cursor const_iterator;
    
    template <class OtherValue>
    struct rebind {
        typedef nary_tree_cursor<OtherValue> other;
    };
    
    nary_tree_cursor()
      : nary_tree_cursor::cursor_adaptor_(0), m_pos(0) {}

    explicit nary_tree_cursor(base_pointer p, size_type pos) 
    : nary_tree_cursor::cursor_adaptor_(p), m_pos(pos) {}

    template <class OtherNode>
    nary_tree_cursor(
        nary_tree_cursor<OtherNode> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherNode*, node_type*>
          , enabler
        >::type = enabler()
    )
    : nary_tree_cursor::cursor_adaptor_(other.base()), m_pos(other.m_pos) {}

     size_type m_pos;

private: 

    friend class iterator_core_access;
    friend class cursor_core_access;
    
    friend class access_detach;
    
    value_type& dereference() const
    {
        return **static_cast<node_type*>(this->base_reference());
    }
    
    bool equal(cursor const& other) const
    {
        return (this->base_reference() == other.base_reference()) && (this->m_pos == other.m_pos);
    }
    
    void increment()
    {
        ++m_pos;
        // this->base_reference() += sizeof(node_type);
    }
    
    void decrement()
    {
        --m_pos;
        //this->base_reference() -= sizeof(node_type);
    }    
    
    void advance(typename nary_tree_cursor::cursor_facade_::difference_type n)
    {
        m_pos += n;
        //this->base_reference() += n * sizeof(node_type);
    }
    
    typename nary_tree_cursor::cursor_facade_::difference_type
    distance_to(nary_tree_cursor z) const //FIXME: convertible to instead of nary_tree_cursor
    {
        return (z.m_pos - this->m_pos);
        //return (z.base_reference() - this->base_reference()) / sizeof(node_type); 
    }
    
    // Container specific
    bool empty_() const
    {
        return this->base_reference()->m_children[m_pos]->empty();
        //return this->base_reference()->get_index();
    }
    
    size_type size_() const
    {
        return this->base_reference()->m_children.size();
    }
    
    size_type max_size_() const
    {
        return this->base_reference()->m_children.max_size();
    }
    
    size_type idx() const
    {
        return m_pos;
        //return 
    }

    void left()
    {
        this->base_reference() = this->base_reference()->m_children[m_pos];
        m_pos  = 0;
        //this->base_reference() = this->base_reference()->operator[0];
    }

    void right()
    {
        size_type new_pos = this->base_reference()->m_children.size()-1; 
        this->base_reference() = this->base_reference()->m_children[m_pos];
        m_pos  = new_pos;
        //this->base_reference() = this->base_reference()->operator[0];
    }

    // Cursor stuff
    void up()
    {
        m_pos  = this->base_reference()->get_index();
        this->base_reference() = static_cast<base_pointer>(this->base_reference()->parent());
        //this->base_reference() = this->base_reference()->parent();
    }
    
protected:
    bool is_on_top() const
    {
        base_pointer parent_begin_node = 
            static_cast<base_pointer>(this->base_reference()->parent())
            ->m_children[this->base_reference()->get_index()];
        return (!m_pos && (this->base_reference() != parent_begin_node));
        // (*this != this->parent().begin())
    }

public:
    
    base_pointer const& base_node() const
    {
        return this->base_reference();
    }

    base_pointer& base_node()
    {
        return this->base_reference();
    }
    
    // TODO: protect?
    void attach(node_pointer p_node)
    {
        p_node->m_parent = this->base_reference();
        
        // Only forest-relevant:
//        p_node->operator[](1) = this->base_reference()->operator[](m_pos);
//        this->base_reference()->operator[](m_pos)->m_parent = p_node;
        
        this->base_reference()->m_children[m_pos] = p_node;
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

template <class Node>
typename detail::nary_tree_cursor<Node>::size_type
index(detail::nary_tree_cursor<Node> const& cur)
{
    return cur.index();
}

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_DETAIL_CURSOR_NARY_HPP
