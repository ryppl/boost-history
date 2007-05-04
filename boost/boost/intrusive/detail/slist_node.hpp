/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gazta�aga  2006-2007
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
#include <boost/intrusive/circular_slist_algorithms.hpp>
#include <boost/get_pointer.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <cstddef>

namespace boost {
namespace intrusive {
namespace detail {

// slist_node_traits can be used with circular_slist_algorithms and supplies
// a slist_node holding the pointers needed for a singly-linked list
// it is used by slist_base_hook and slist_member_hook
template<class VoidPointer>
struct slist_node_traits
{
   struct node;

   typedef typename boost::pointer_to_other
      <VoidPointer, node>::type          node_ptr;
   typedef typename boost::pointer_to_other
      <VoidPointer, const node>::type    const_node_ptr;

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
template<class T, class ValueTraits>
class slist_iterator
  : public boost::iterator_facade
         < slist_iterator<T, ValueTraits>
         , T
         , boost::forward_traversal_tag
         , T&
         , typename std::iterator_traits<typename ValueTraits::node_ptr>::difference_type
         >
{
   typedef typename ValueTraits::node_traits    node_traits;
   typedef typename node_traits::node           node;
   typedef typename node_traits::node_ptr       node_ptr;
   typedef typename node_traits::const_node_ptr const_node_ptr;
   struct enabler{};

   public:
   typedef typename pointer_to_other<typename ValueTraits::node_ptr, T>::type pointer;
   typedef typename std::iterator_traits<node_ptr>::difference_type difference_type;

   slist_iterator ()
      :  node_ (0)
   {}

   explicit slist_iterator(node_ptr node)
      :  node_ (node)
   {}

   template <class OtherValue>
   slist_iterator(slist_iterator<OtherValue, ValueTraits> const& other
                 ,typename boost::enable_if<
                        boost::is_convertible<OtherValue*,T*>
                     , enabler
                     >::type = enabler()
                  )
      :  node_(other.pointed_node())
   {}

   const node_ptr &pointed_node() const
   { return node_; }

   private:
   friend class boost::iterator_core_access;
   template <class, class> friend class slist_iterator;

   template <class OtherValue>
   bool equal(slist_iterator<OtherValue, ValueTraits> const& other) const
   {  return other.pointed_node() == node_;  }

   void increment()
   {  node_ = node_traits::get_next(node_);   }

   T& dereference() const
   { return *ValueTraits::to_value_ptr(node_); }

   node_ptr node_;
};

} //namespace detail 
} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_SLIST_NODE_HPP
