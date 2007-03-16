/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2007.
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_SLIST_NODE_HPP
#define BOOST_INTRUSIVE_SLIST_NODE_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/slist_algorithms.hpp>
#include <boost/get_pointer.hpp>
#include <cstddef>

namespace boost {
namespace intrusive {
namespace detail {

// slist_node_traits can be used with slist_algorithms and supplies
// a list_node holding the pointers needed for a double-linked list
// it is used by islist_derived_node and islist_member_node
template<class VoidPointer>
struct slist_node_traits
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
      node_ptr next_;
   };

   static node_ptr get_next(const_node_ptr n)
   {  return n->next_;  }  

   static void set_next(node_ptr n, node_ptr next)
   {  n->next_ = next;  }  
};


// slist_iterator provides some basic functions for a 
// node oriented forward iterator:
template<class T, class Self, class NodeTraits>
class slist_iterator
   :  public std::iterator<std::forward_iterator_tag, T>
{
   protected:
   typedef typename NodeTraits::node                  node;
   typedef typename detail::pointer_to_other
      <typename NodeTraits::void_pointer, node>::type node_ptr;

   public:
   slist_iterator ()
      :  node_ (0)
   {}

   explicit slist_iterator (node_ptr node)
      :  node_ (node)
   {}

   Self& operator++() 
   { 
      node_ = NodeTraits::get_next(node_); 
      return static_cast<Self&> (*this); 
   }
   
   Self operator++(int)
   {
      Self result (node_);
      node_ = NodeTraits::get_next(node_);
      return result;
   }

   bool operator== (const Self& i) const
   { return node_ == i.list_node(); }

   bool operator!= (const Self& i) const
   { return !operator== (i); }

   node_ptr list_node() const
   { return node_; }

   Self next() const
   {  return Self(NodeTraits::get_next(node_));  }  

   Self &operator=(const node_ptr &node)
   {  node_ = node;  return static_cast<Self&>(*this);  }

   private:
   node_ptr node_;
};

} //namespace detail 
} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_SLIST_NODE_HPP
