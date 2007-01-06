/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztañaga  2006-2007.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
/* The internal implementation of red-black trees is based on that of SGI STL
 * stl_tree.h file: 
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.*/

#ifndef BOOST_INTRUSIVE_RBTREE_NODE_HPP
#define BOOST_INTRUSIVE_RBTREE_NODE_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <boost/get_pointer.hpp>
//#include <boost/interprocess/offset_ptr.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <cstddef>
#include <boost/detail/no_exceptions_support.hpp>

namespace boost {
namespace intrusive {
namespace detail {

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                Generic node_traits for any pointer type                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//This is the default implementation, 3 generic pointers plus an enum
template<class VoidPointer>
struct default_rbtree_node_traits_impl
{
   struct node;
   typedef VoidPointer void_pointer;

   private:
   typedef typename detail::pointer_to_other
      <VoidPointer, node>::type          node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const node>::type    const_node_ptr;

   public:
   struct node
   {
      enum color { red_t, black_t };

      static color black()
      {  return black_t;  }

      static color red()
      {  return red_t;  }

      node_ptr parent_, left_, right_;
      color color_;
   };

   typedef typename node::color color;

   static node_ptr get_parent(const_node_ptr n)
   {  return n->parent_;  }

   static void set_parent(node_ptr n, node_ptr p)
   {  n->parent_ = p;  }

   static node_ptr get_left(const_node_ptr n)
   {  return n->left_;  }

   static void set_left(node_ptr n, node_ptr l)
   {  n->left_ = l;  }

   static node_ptr get_right(const_node_ptr n)
   {  return n->right_;  }

   static void set_right(node_ptr n, node_ptr r)
   {  n->right_ = r;  }

   static color get_color(const_node_ptr n)
   {  return n->color_;  }

   static void set_color(node_ptr n, color c)
   {  n->color_ = c;  }

   static color black()
   {  return node::black_t;  }

   static color red()
   {  return node::red_t;  }
};

//The default possibility is the generic implementation
template<class VoidPointer>
struct rbtree_node_traits
   :  public default_rbtree_node_traits_impl<VoidPointer>
{};

//This is the compact representation: 3 pointers
template<class VoidPointer>
struct compact_node
{
   typedef typename detail::pointer_to_other
      <VoidPointer, compact_node>::type          node_ptr;   

   enum color { red_t, black_t };

   static color black()
   {  return black_t;  }

   static color red()
   {  return red_t;  }

   node_ptr parent_, left_, right_;
};

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//          Generic node_traits specialization for raw pointers            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////



//This specialization, embeds the color in the parent pointer
//so we save one word per node
template <bool TwoByteAlignment>
struct rbtree_node_traits_void_ptr
{
   typedef compact_node<void*> node;
   typedef void* void_pointer;

   private:
   typedef node *       node_ptr;
   typedef const node * const_node_ptr;

   public:
   typedef node::color color;

   static node_ptr get_parent(const_node_ptr n)
   {  return (node_ptr)((std::size_t)n->parent_ & ~1);  }

   static void set_parent(node_ptr n, node_ptr p)
   {  n->parent_ = (node_ptr)((std::size_t)p | ((std::size_t)n->parent_ & 1));  }

   static node_ptr get_left(const_node_ptr n)
   {  return n->left_;  }

   static void set_left(node_ptr n, node_ptr l)
   {  n->left_ = l;  }

   static node_ptr get_right(const_node_ptr n)
   {  return n->right_;  }

   static void set_right(node_ptr n, node_ptr r)
   {  n->right_ = r;  }

   static color get_color(const_node_ptr n)
   {  return (color)((std::size_t)n->parent_ & 1);  }

   static void set_color(node_ptr n, color c)
   {  n->parent_ = (node_ptr)((std::size_t)get_parent(n) | (std::size_t)c);  }

   static color black()
   {  return node::black_t;  }

   static color red()
   {  return node::red_t;  }
};

template<>
struct rbtree_node_traits_void_ptr<false>
   :  public default_rbtree_node_traits_impl<void*>
{};

//This specialization will check if the pointer optimization is
//possible with raw pointers
template<>
struct rbtree_node_traits<void*>
   :  public rbtree_node_traits_void_ptr
      <((boost::alignment_of< compact_node<void*> >::value % 2) == 0)>
{};
/*
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//       Generic node_traits specialization for offset_ptr pointers        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//This specialization, embeds the color in the parent pointer
//so we save one word per node.
//offset_ptr, uses the offset between the pointer and the pointee
//as an internal member. The 1 byte offset is defined as a null pointer
//so, unlike raw pointers, so if nodes are aligned to 4 bytes,
//we will use the second bit as the color mark.
template <bool FourByteAlignment>
struct rbtree_node_traits_offset_ptr_void
{
   typedef boost::interprocess::offset_ptr<void> void_pointer;
   typedef compact_node<void_pointer> node;

   private:
   typedef boost::interprocess::offset_ptr<node>         node_ptr;
   typedef boost::interprocess::offset_ptr<const node>   const_node_ptr;

   public:

   typedef node::color color;

   static node_ptr get_parent(const_node_ptr n)
   {  return node_ptr((node*)((std::size_t)n->parent_.get() & ~2));  }

   static void set_parent(node_ptr n, node_ptr p)
   {
      n->parent_ = (node *)(((std::size_t)p.get()) | (((std::size_t)n->parent_.get()) & 2)); 
   }

   static node_ptr get_left(const_node_ptr n)
   {  return n->left_;  }

   static void set_left(node_ptr n, node_ptr l)
   {  n->left_ = l;  }

   static node_ptr get_right(const_node_ptr n)
   {  return n->right_;  }

   static void set_right(node_ptr n, node_ptr r)
   {  n->right_ = r;  }

   static color get_color(const_node_ptr n)
   {  return (color)(((std::size_t)n->parent_.get() & 2) >> 1);  }

   static void set_color(node_ptr n, color c)
   {  n->parent_ = (node *)((std::size_t)get_parent(n).get() | ((std::size_t)c << 1));  }

   static color black()
   {  return node::black_t;  }

   static color red()
   {  return node::red_t;  }
};

//This is the specialization for nodes with no 4 byte alignment
template <>
struct rbtree_node_traits_offset_ptr_void<false>
   :  public default_rbtree_node_traits_impl< boost::interprocess::offset_ptr<void> >
{};

//This specialization will check if the pointer optimization is
//possible with offset_ptr pointers
template<>
struct rbtree_node_traits< boost::interprocess::offset_ptr<void> >
   :  public rbtree_node_traits_offset_ptr_void
         <((boost::alignment_of
               < compact_node< boost::interprocess::offset_ptr<void> > > 
            ::value % 4) == 0)
         >
{};
*/
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                A base class for the rbtree container                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// rbtree_iterator provides some basic functions for a 
// rb tree node oriented bidirectional iterator:
template<class T, class Self, class NodeTraits>
class rbtree_iterator
   :  public std::iterator<std::bidirectional_iterator_tag, T>
{
   typedef rbtree_algorithms<NodeTraits> sequence_algorithms;

   protected:
   typedef typename NodeTraits::node            node;
   typedef typename detail::pointer_to_other
      <typename NodeTraits::void_pointer
      ,node>::type                           node_ptr;

   public:
   rbtree_iterator ()
      :  node_ (0)
   {}

   explicit rbtree_iterator (node_ptr node)
      :  node_ (node)
   {}

   Self& operator++()
   {
      node_ = sequence_algorithms::next_node(node_);
      return static_cast<Self&> (*this);
   }

   Self operator++(int)
   {
      Self result (node_);
      node_ = sequence_algorithms::next_node (node_);
      return result;
   }

   Self& operator--()
   {
      node_ = sequence_algorithms::prev_node (node_);
      return static_cast<Self&> (*this);
   }

   Self operator--(int)
   {
      Self result (node_);
      node_ = sequence_algorithms::prev_node (node_);
      return result;
   }

   bool operator== (const Self& i) const
   { return node_ == i.tree_node(); }

   bool operator!= (const Self& i) const
   { return !operator== (i); }

   node_ptr tree_node() const
   { return node_; }

   Self left() const
   {  return Self(NodeTraits::get_left(node_));  }  

   Self right() const
   {  return Self(NodeTraits::get_right(node_));  }  

   Self parent() const
   {  return Self(NodeTraits::get_parent(node_));  }  

   private:
   node_ptr node_;
};


} //namespace detail 
} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_RBTREE_NODE_HPP
