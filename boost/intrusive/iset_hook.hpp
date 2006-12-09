/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_ISET_HOOK_HPP
#define BOOST_INTRUSIVE_ISET_HOOK_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/detail/rbtree_node.hpp>
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <boost/intrusive/value_traits_type.hpp>
#include <boost/get_pointer.hpp>
#include <stdexcept>

namespace boost {
namespace intrusive {

//! Derive a class from iset_base_hook in order to store objects in 
//! in an iset/imultiset. iset_base_hook holds the data necessary to maintain 
//! the set/multiset and provides an appropriate value_traits class for iset/imultiset.
//! 
//! The first integer template argument defines a tag to identify the node. 
//! The same tag value can be used in different classes, but if a class is 
//! derived from more then one iset_base_hook, then each iset_base_hook needs its 
//! unique tag.
//!
//! The second boolean template argument SafeMode controls initializes
//! the node to a safe state in the constructor and asserts if the node is destroyed
//! or it's assigned but it's still inserted in a iset/imultiset.
//!
//! The third argument is the pointer type that will be used internally in the hook
//! and the iset/imultiset configured from this hook.
template<int Tag, bool SafeMode = true, class VoidPointer = void*>
class iset_base_hook
   :  private detail::rbtree_node_traits<VoidPointer>::node
{
   public:
   typedef detail::rbtree_node_traits<VoidPointer>    node_traits;
   enum { type = SafeMode? safe_mode_value_traits : normal_value_traits};

   private:
   typedef rbtree_algorithms<node_traits>     sequence_algorithms;
   typedef typename node_traits::node                 node;
   typedef typename detail::pointer_to_other
      <VoidPointer, node>::type                       node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const node>::type                 const_node_ptr;
   typedef iset_base_hook 
      <Tag, SafeMode, VoidPointer>                    this_type;

   typedef typename detail::pointer_to_other
      <VoidPointer, this_type >::type                 this_type_ptr;

   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type >::type           const_this_type_ptr;

   node_ptr this_as_node()
   {  return node_ptr(static_cast<node *const>(this)); }

   const_node_ptr this_as_node() const
   {  return const_node_ptr(static_cast<const node *const>(this)); }

   public:

   //! <b>Effects</b>: If SafeMode is true the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using iset_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_base_hook()
      :  node()
   {
      if(SafeMode){
         sequence_algorithms::init(this_as_node());
      }
   }

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using iset_base_hook STL-compliant without forcing the 
   //!   user to do some additional work. "swap" can be used to emulate
   //!   move-semantics.
   iset_base_hook(const iset_base_hook& )
      :  node()
   {
      if(SafeMode){
         sequence_algorithms::init(this_as_node());
      }
   }

   //! <b>Effects</b>: If safe-mode is true
   //!   Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using iset_base_hook STL-compliant without forcing the 
   //!   user to do some additional work. "swap" can be used to emulate
   //!   move-semantics.
   iset_base_hook& operator=(const iset_base_hook& ) 
   {
      if(SafeMode){
         BOOST_ASSERT(!this->linked());
      }
      return *this; 
   }

   //! <b>Effects</b>: If SafeMode is set to false, the destructor does
   //!   nothing (ie. no code is generated). If SafeMode is true and the
   //!   object is stored in an iset an assertion is raised.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~iset_base_hook() 
   {
      if(SafeMode){
         BOOST_ASSERT(!this->linked());
      }
   }

   //! <b>Precondition</b>: The hook must be in safe-mode.
   //!
   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether iset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool linked() const 
   {
      //linked() can be only used in safe-mode
      BOOST_ASSERT(SafeMode);
      return !sequence_algorithms::unique(this_as_node()); 
   }

   //! The value_traits class is used as the first template argument for imultiset. 
   //! The template argument T defines the class type stored in imultiset. Objects 
   //! of type T and of types derived from T can be stored. T don't need to be 
   //! copy-constructible or assignable.
   template<class T>
   struct value_traits
      :  detail::derivation_value_traits<T, this_type, Tag>
   {};

   //! <b>Effects</b>: Converts a pointer to a node stored in a sequence into
   //!   a pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static this_type_ptr to_hook(node_ptr p)
   {
      using boost::get_pointer;
      return this_type_ptr(static_cast<iset_base_hook*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook(const_node_ptr p)
   {
      using boost::get_pointer;
      return const_this_type_ptr(static_cast<const iset_base_hook*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   node_ptr to_node_ptr()
   { return this_as_node(); }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   const_node_ptr to_node_ptr() const
   { return this_as_node(); }
};

//! Derive a class from iset_auto_base_hook in order to store objects in in an 
//! iset/imultiset. iset_auto_base_hook holds the data necessary to maintain the 
//! set and provides an appropriate value_traits class for iset/imultiset.
//!
//! The difference between iset_auto_base_hook and iset_base_hook is that
//! iset_auto_base_hook removes itself automatically from the container
//! in the assignment operator and the destructor. It also provides a new
//! "unlink_self" method so that the user can unlink its class without using
//! the container.
//!
//! iset_auto_base_hook can only be used with non constant-time iset/imultisets 
//! because only those sets can support the auto-unlink feature.
//! 
//! The first integer template argument defines a tag to identify the node. 
//! The same tag value can be used in different classes, but if a class is 
//! derived from more then one iset_auto_base_hook, then each iset_auto_base_hook needs its 
//! unique tag.
//!
//! The second argument is the pointer type that will be used internally in the hook
//! and the iset/multiset configured from this hook.
template<int Tag, class VoidPointer = void*>
class iset_auto_base_hook
   :  private detail::rbtree_node_traits<VoidPointer>::node
{
   public:
   typedef detail::rbtree_node_traits<VoidPointer>    node_traits;
   enum { type = auto_unlink_value_traits  };

   private:
   typedef rbtree_algorithms<node_traits>     sequence_algorithms;
   typedef typename node_traits::node                 node;
   typedef typename detail::pointer_to_other
      <VoidPointer, node>::type                       node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const node>::type                 const_node_ptr;
   typedef iset_auto_base_hook 
      <Tag, VoidPointer>                              this_type;

   typedef typename detail::pointer_to_other
      <VoidPointer, this_type >::type                 this_type_ptr;

   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type >::type           const_this_type_ptr;

   node_ptr this_as_node()
   {  return node_ptr(static_cast<node *const>(this)); }

   const_node_ptr this_as_node() const
   {  return const_node_ptr(static_cast<const node *const>(this)); }

   public:

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using iset_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_auto_base_hook()
      :  node()
   {  sequence_algorithms::init(this_as_node());   }

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using iset_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_auto_base_hook(const iset_auto_base_hook& ) 
      :  node()
   {  sequence_algorithms::init(this_as_node());   }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //!   The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using iset_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_auto_base_hook& operator=(const iset_auto_base_hook& ) 
   {  this->unlink_self();  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~iset_auto_base_hook() 
   {  this->unlink_self();  }

   //! <b>Effects</b>: Swapping two nodes swaps the position of the elements 
   //!   related to that nodes in one or two sequences. That is, if the node 
   //!   this is part of the element e1, the node x is part of the element e2 
   //!   and both elements are included in the sequences s1 and s2, then after 
   //!   the swap-operation e1 is in s2 at the position of e2 and e2 is in s1 
   //!   at the position of e1. If one element is not in a sequence, then 
   //!   after the swap-operation the other element is not in a sequence. 
   //!   Iterators to e1 and e2 related to that nodes are invalidated. 
   //!
   //! <b>Complexity</b>: Constant 
   //!
   //! <b>Throws</b>: Nothing. 
   void swap_nodes(iset_auto_base_hook& other) 
   { sequence_algorithms::swap_nodes(this_as_node(), other.this_as_node()); }

   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether iset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool linked() const 
   {  return !sequence_algorithms::unique(this_as_node());  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink_self()
   {
      sequence_algorithms::unlink_and_rebalance(this_as_node());
      sequence_algorithms::init(this_as_node());
   }

   //! The value_traits class is used as the first template argument for iset. 
   //! The template argument T defines the class type stored in iset. Objects 
   //! of type T and of types derived from T can be stored. T doesn't need to be 
   //! copy-constructible or assignable.
   template<class T>
   struct value_traits
      : detail::derivation_value_traits<T, this_type, Tag>
   {};

   //! <b>Effects</b>: Converts a pointer to a node stored in a sequence into
   //!   a pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static this_type_ptr to_hook(node_ptr p)
   {
      using boost::get_pointer;
      return this_type_ptr(static_cast<iset_auto_base_hook*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook(const_node_ptr p)
   {
      using boost::get_pointer;
      return const_this_type_ptr(static_cast<const iset_auto_base_hook*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   node_ptr to_node_ptr()
   { return this_as_node(); }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   const_node_ptr to_node_ptr() const
   { return this_as_node(); }
};


//! Put a public data member iset_member_hook in order to store objects of this class in
//! an iset/imultiset. iset_member_hook holds the data necessary for maintaining the
//! set/multiset and provides an appropriate value_traits class for iset/imultiset.
//! 
//! The template argument T defines the class type stored in iset/imultiset. Objects of 
//! type T and of types derived from T can be stored. T doesn't need to be 
//! copy-constructible or assignable.
//! 
//! The second boolean template argument SafeMode controls initializes
//! the node to a safe state in the constructor and asserts if the node is destroyed
//! or it's assigned but it's still inserted in a iset/imultiset.
//!
//! The third argument is the pointer type that will be used internally in the hook
//! and the iset/imultiset configured from this hook.
template<class T, bool SafeMode = true, class VoidPointer = void*>
class iset_member_hook
   :  private detail::rbtree_node_traits<VoidPointer>::node
{
   public:
   typedef detail::rbtree_node_traits<VoidPointer>       node_traits;
   enum { type = SafeMode? safe_mode_value_traits : normal_value_traits};

   private:
   typedef rbtree_algorithms<node_traits>        sequence_algorithms;
   typedef typename node_traits::node                    node;
   typedef typename detail::pointer_to_other
      <VoidPointer, node>::type                          node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const node>::type                    const_node_ptr;
   typedef iset_member_hook
      <T, SafeMode, VoidPointer>                         this_type;
   typedef typename detail::pointer_to_other
      <VoidPointer, T>::type                             pointer;

   typedef typename detail::pointer_to_other
      <VoidPointer, this_type >::type                    this_type_ptr;

   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type >::type              const_this_type_ptr;

   node_ptr this_as_node()
   {  return node_ptr(static_cast<node *const>(this)); }

   const_node_ptr this_as_node() const
   {  return const_node_ptr(static_cast<const node *const>(this)); }

   public:
   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument given to the copy-constructor or 
   //!   assignment operator is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using iset_member_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_member_hook()
      :  node()
   {
      if(SafeMode){
         sequence_algorithms::init(this_as_node());
      }
   }

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using iset_member_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_member_hook(const iset_member_hook& )
      :  node()
   {
      if(SafeMode){
         sequence_algorithms::init(this_as_node());
      }
   }

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using iset_member_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_member_hook& operator=(const iset_member_hook& ) 
   {
      if(SafeMode){
         BOOST_ASSERT(!this->linked());
      }
      return *this;
   }

   //! <b>Effects</b>: If SafeMode is set to false, the destructor does
   //!   nothing (ie. no code is generated). Otherwise, if the object is 
   //!   stored in an imultiset using the appropriate node, the object is removed 
   //!   from that imultiset.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~iset_member_hook() 
   { 
      if(SafeMode){
         BOOST_ASSERT(!this->linked());
      }
   }

   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether iset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant
   bool linked() const 
   {
      //We must be in safe-mode to know if we are really linked
      //Otherwise, this would lead to an unknown state
      BOOST_ASSERT(SafeMode);
      return !sequence_algorithms::unique(this_as_node());
   }

   //! The value_traits class is used as the first template argument for imultiset. 
   //! The template argument is a pointer to member pointing to the node in 
   //! the class. Objects of type T and of types derived from T can be stored. 
   //! T don't need to be copy-constructible or assignable.
   template<this_type T::* P>
   struct value_traits
      :  detail::member_value_traits<T, this_type, P>
   {};

   //! <b>Effects</b>: Converts a pointer to a node stored in a sequence into
   //!   a pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static this_type_ptr to_hook(node_ptr p)
   {
      using boost::get_pointer;
      return this_type_ptr(static_cast<this_type*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook(const_node_ptr p)
   {
      using boost::get_pointer;
      return const_this_type_ptr(static_cast<const this_type*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   node_ptr to_node_ptr()
   { return this_as_node(); }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   const_node_ptr to_node_ptr() const
   { return this_as_node(); }
};

//! Put a public data member iset_auto_member_hook in order to store objects of this 
//! class in an iset/imultiset. iset_auto_member_hook holds the data necessary for
//! maintaining the set/multiset and provides an appropriate value_traits class for
//! iset/imultiset.
//!
//! The difference between iset_auto_member_hook and iset_auto_member_hook is that
//! iset_auto_member_hook removes itself automatically from the container
//! in the assignment operator and the destructor. It also provides a new
//! "unlink_self" method so that the user can unlink its class without using
//! the container.
//!
//! iset_auto_member_hook can only be used with non constant-time iset/imultisets 
//! because only those set/multisets can support the auto-unlink feature.
//! 
//! The first template argument T defines the class type stored in iset/imultisets.
//! Objects of type T and of types derived from T can be stored. T doesn't need to 
//! be copy-constructible or assignable.
//!
//! The second argument is the pointer type that will be used internally in the hook
//! and the iset/imultiset configured from this hook.
template<class T, class VoidPointer = void *>
class iset_auto_member_hook
   :  private detail::rbtree_node_traits<VoidPointer>::node
{
   public:
   typedef detail::rbtree_node_traits<VoidPointer>      node_traits;
   enum { type = auto_unlink_value_traits  };

   private:
   typedef rbtree_algorithms<node_traits>        sequence_algorithms;
   typedef typename detail::pointer_to_other
      <VoidPointer, T>::type                          pointer;
   typedef typename node_traits::node                 node;
   typedef typename detail::pointer_to_other
      <VoidPointer, node>::type                       node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const node>::type                 const_node_ptr;
   typedef iset_auto_member_hook
      <T, VoidPointer>                                this_type;

   typedef typename detail::pointer_to_other
      <VoidPointer, this_type >::type                 this_type_ptr;

   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type >::type           const_this_type_ptr;

   node_ptr this_as_node()
   {  return node_ptr(static_cast<node *const>(this)); }

   const_node_ptr this_as_node() const
   {  return const_node_ptr(static_cast<const node *const>(this)); }

   public:
   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using iset_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_auto_member_hook()
      :  node()
   {  sequence_algorithms::init(this_as_node());   }

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using iset_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_auto_member_hook(const iset_auto_member_hook& ) 
      :  node()
   {  sequence_algorithms::init(this_as_node());   }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //!   The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using iset_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   iset_auto_member_hook& operator=(const iset_auto_member_hook& )
   {  this->unlink_self();  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~iset_auto_member_hook()
   {  this->unlink_self();  }

   //! <b>Effects</b>: Swapping two nodes swaps the position of the elements 
   //!   related to that nodes in one or two sequences. That is, if the node 
   //!   this is part of the element e1, the node x is part of the element e2 
   //!   and both elements are included in the sequences s1 and s2, then after 
   //!   the swap-operation e1 is in s2 at the position of e2 and e2 is in s1 
   //!   at the position of e1. If one element is not in a sequence, then 
   //!   after the swap-operation the other element is not in a sequence. 
   //!   Iterators to e1 and e2 related to that nodes are invalidated. 
   //!
   //! <b>Complexity</b>: Constant 
   //!
   //! <b>Throws</b>: Nothing.
   void swap_nodes(iset_auto_member_hook& other) 
   { sequence_algorithms::swap_nodes(this_as_node(), other.this_as_node()); }

   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether iset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool linked() const 
   {  return !sequence_algorithms::unique(this_as_node());  }

   //! The value_traits class is used as the first template argument for iset. 
   //! The template argument is a pointer to member pointing to the node in 
   //! the class. Objects of type T and of types derived from T can be stored. 
   //! T doesn't need to be copy-constructible or assignable.
   template<this_type T::* M>
   struct value_traits
      : detail::member_value_traits<T, this_type, M>
   {};

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink_self()
   {
      sequence_algorithms::unlink_and_rebalance(this_as_node());
      sequence_algorithms::init(this_as_node());
   }

   //! <b>Effects</b>: Converts a pointer to a node stored in a sequence into
   //!   a pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static this_type_ptr to_hook(node_ptr p)
   {
      using boost::get_pointer;
      return this_type_ptr(static_cast<this_type*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook(const_node_ptr p)
   {
      using boost::get_pointer;
      return const_this_type_ptr(static_cast<const this_type*> (get_pointer(p))); 
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   node_ptr to_node_ptr()
   { return this_as_node(); }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   const_node_ptr to_node_ptr() const
   { return this_as_node(); }
};


/*
template<class T
        ,class PointerToT
        ,class ColorT
        ,ColorT T::* Color
        ,ColorT Black
        ,ColorT Red
        ,PointerToT T::* Parent
        ,PointerToT T::* Left
        ,PointerToT T::* Right
        ,bool SafeMode = true>
struct iset_ptr_to_member_hook
{
   typedef T            value_type;
   typedef PointerToT   pointer;
   enum { SafeMode = SafeMode };

   private:
   typedef PointerToT                        node_ptr;
   typedef typename detail::pointer_to_other
      <PointerToT, const T>::type            const_node_ptr;
   typedef const_node_ptr                    const_pointer;

   public:

   struct node_traits
   {
      typedef T                                 node;
      typedef typename detail::pointer_to_other
         <PointerToT, void>::type               void_pointer;
      typedef ColorT                            color;

      private:
      typedef PointerToT                        node_ptr;
      typedef typename detail::pointer_to_other
         <PointerToT, const T>::type            const_node_ptr;

      public:
      static node_ptr get_parent(const_node_ptr n)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         return get_pointer(n)->*Parent;
      }

      static void set_parent(node_ptr n, node_ptr p)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         get_pointer(n)->*Parent = p;
      }

      static node_ptr get_left(const_node_ptr n)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         return get_pointer(n)->*Left;
      }

      static void set_left(node_ptr n, node_ptr l)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         get_pointer(n)->*Left = l;
      }

      static node_ptr get_right(const_node_ptr n)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         return get_pointer(n)->*Right;
      }

      static void set_right(node_ptr n, node_ptr r)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         get_pointer(n)->*Right = r;
      }

      static color get_color(const_node_ptr n)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         return get_pointer(n)->*Color;
      }

      static void set_color(node_ptr n, color c)
      {
         //Can we overload "operator ->*" for smart pointers?
         //return n->*Parent;
         using boost::get_pointer;
         get_pointer(n)->*Color = c;
      }

      static color black()
      {  return Black;  }

      static color red()
      {  return Red;  }
   };
  
   static node_ptr to_node_ptr(value_type& v) 
   {  return &v;  }

   static const_node_ptr to_node_ptr(const value_type& v) 
   {  return &v;  }
  
   static pointer to_value_ptr(node_ptr n)
   {  return n;  }

   static const_pointer to_value_ptr(const_node_ptr n)
   {  return n;  }

   static void init_node(typename node_traits::node& data)
   {  rbtree_algorithms<node_traits>::init(&data);  }  

   static bool linked(typename node_traits::node& data)
   {  rbtree_algorithms<node_traits>::unique(&data);  }  

   static void unlink_node(typename node_traits::node& data)
   {  rbtree_algorithms<node_traits>::unlink_and_rebalance(&data);  }
};
*/
} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_ISET_HOOK_HPP
