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

#ifndef BOOST_INTRUSIVE_IHASHSET_HOOK_HPP
#define BOOST_INTRUSIVE_IHASHSET_HOOK_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
//#include <boost/intrusive/detail/rbtree_node.hpp>
//#include <boost/intrusive/rbtree_algorithms.hpp>
#include <boost/intrusive/islist_hook.hpp>
#include <boost/intrusive/value_traits_type.hpp>
#include <boost/get_pointer.hpp>
#include <stdexcept>

namespace boost {
namespace intrusive {

//! Derive a class from ihashset_base_hook in order to store objects in 
//! in an ihashset/imultihashset. ihashset_base_hook holds the data necessary to maintain 
//! the hashset/multihashset and provides an appropriate value_traits class for ihashset/imultihashset.
//! 
//! The first integer template argument defines a tag to identify the node. 
//! The same tag value can be used in different classes, but if a class is 
//! derived from more then one ihashset_base_hook, then each ihashset_base_hook needs its 
//! unique tag.
//!
//! The second boolean template argument SafeMode controls initializes
//! the node to a safe state in the constructor and asserts if the node is destroyed
//! or it's assigned but it's still inserted in a ihashset/imultihashset.
//!
//! The third argument is the pointer type that will be used internally in the hook
//! and the ihashset/imultihashset configured from this hook.
template<int Tag, bool SafeMode = true, class VoidPointer = void*>
class ihashset_base_hook
{
   typedef islist_base_hook<Tag, SafeMode, VoidPointer> IsListHook;
   IsListHook m_islisthook;
   typedef ihashset_base_hook
      <Tag, SafeMode, VoidPointer>                    this_type;

   public:
   enum { type = IsListHook::type };

   typedef typename IsListHook::node_traits           node_traits;
   typedef typename IsListHook::node_ptr              node_ptr;
   typedef typename IsListHook::const_node_ptr        const_node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, this_type>::type                  this_type_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type>::type            const_this_type_ptr;

   //! <b>Effects</b>: If SafeMode is true the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using islist_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_base_hook()
      :  m_islisthook()
   {}

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using islist_base_hook STL-compliant without forcing the 
   //!   user to do some additional work. "swap" can be used to emulate
   //!   move-semantics.
   ihashset_base_hook(const ihashset_base_hook &other)
      :  m_islisthook(other.m_islisthook)
   {}

   //! <b>Effects</b>: If safe-mode is true
   //!   Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using islist_base_hook STL-compliant without forcing the 
   //!   user to do some additional work. "swap" can be used to emulate
   //!   move-semantics.
   ihashset_base_hook& operator=(const ihashset_base_hook &other)
   {  m_islisthook = other.m_islisthook;  return *this;  }

   //! <b>Effects</b>: If SafeMode is set to false, the destructor does
   //!   nothing (ie. no code is generated). If SafeMode is true and the
   //!   object is stored in an ihashset/ihashmultiset an assertion is raised.
   //! 
   //! <b>Throws</b>: Nothing.
   ~ihashset_base_hook() 
   {}

   //! <b>Precondition</b>: The hook must be in safe-mode.
   //!
   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether ihashset/ihashmultiset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant
   bool linked() const 
   {  return m_islisthook.linked(); }

   //! The value_traits class is used as the first template argument for ihashset/ihashmultiset. 
   //! The template argument T defines the class type stored in ihashset/ihashmultiset. Objects 
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
   static this_type_ptr to_hook_ptr(node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return this_type_ptr((this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing.
   static const_this_type_ptr to_hook_ptr(const_node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return const_this_type_ptr((const this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing.
   node_ptr to_node_ptr()
   {  return m_islisthook.to_node_ptr();  }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing.
   const_node_ptr to_node_ptr() const
   {  return m_islisthook.to_node_ptr();  }
};


//! Derive a class from ihashset_auto_base_hook in order to store objects in in an 
//! ihashset/imultihashset. ihashset_auto_base_hook holds the data necessary to maintain the 
//! hashset and provides an appropriate value_traits class for ihashset/imultihashset.
//!
//! The difference between ihashset_auto_base_hook and ihashset_base_hook is that
//! ihashset_auto_base_hook removes itself automatically from the container
//! in the assignment operator and the destructor. It also provides a new
//! "unlink_self" method so that the user can unlink its class without using
//! the container.
//!
//! ihashset_auto_base_hook can only be used with non constant-time ihashset/imultihashsets 
//! because only those hashsets can support the auto-unlink feature.
//! 
//! The first integer template argument defines a tag to identify the node. 
//! The same tag value can be used in different classes, but if a class is 
//! derived from more then one ihashset_auto_base_hook, then each ihashset_auto_base_hook needs its 
//! unique tag.
//!
//! The second argument is the pointer type that will be used internally in the hook
//! and the ihashset/multihashset configured from this hook.
template<int Tag, class VoidPointer = void*>
class ihashset_auto_base_hook
{
   typedef islist_auto_base_hook<Tag, VoidPointer> IsListHook;
   IsListHook m_islisthook;
   typedef ihashset_auto_base_hook
      <Tag, VoidPointer>                    this_type;

   public:
   enum { type = IsListHook::type };

   typedef typename IsListHook::node_traits           node_traits;
   typedef typename IsListHook::node_ptr              node_ptr;
   typedef typename IsListHook::const_node_ptr        const_node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, this_type>::type                  this_type_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type>::type            const_this_type_ptr;

   public:

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using islist_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_auto_base_hook()
      :  m_islisthook()
   {}

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using islist_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_auto_base_hook(const ihashset_auto_base_hook &other)
      :  m_islisthook(other.m_islisthook)
   {}

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //!   The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using islist_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_auto_base_hook& operator=(const ihashset_auto_base_hook &other)
   {  m_islisthook = other.m_islisthook;  return *this;  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~ihashset_auto_base_hook() 
   {}

   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether ihashset/ihashmultiset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool linked() const 
   {  return m_islisthook.linked();  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink_self()
   {  return m_islisthook.unlink_self();  }

   //! The value_traits class is used as the first template argument for ihashset/ihashmultiset. 
   //! The template argument T defines the class type stored in ihashset/ihashmultiset. Objects 
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
   static this_type_ptr to_hook_ptr(node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return this_type_ptr((this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook_ptr(const_node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return const_this_type_ptr((const this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing.
   node_ptr to_node_ptr()
   {  return m_islisthook.to_node_ptr();  }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing.
   const_node_ptr to_node_ptr() const
   {  return m_islisthook.to_node_ptr();  }
};


//! Put a public data member ihashset_member_hook in order to store objects of this class in
//! an ihashset/imultihashset. ihashset_member_hook holds the data necessary for maintaining the
//! hashset/multihashset and provides an appropriate value_traits class for ihashset/imultihashset.
//! 
//! The template argument T defines the class type stored in ihashset/imultihashset. Objects of 
//! type T and of types derived from T can be stored. T doesn't need to be 
//! copy-constructible or assignable.
//! 
//! The second boolean template argument SafeMode controls initializes
//! the node to a safe state in the constructor and asserts if the node is destroyed
//! or it's assigned but it's still inserted in a ihashset/imultihashset.
//!
//! The third argument is the pointer type that will be used internally in the hook
//! and the ihashset/imultihashset configured from this hook.
template<class T, bool SafeMode = true, class VoidPointer = void*>
class ihashset_member_hook
{
   typedef islist_member_hook<T, SafeMode, VoidPointer> IsListHook;
   IsListHook m_islisthook;
   typedef ihashset_member_hook
      <T, SafeMode, VoidPointer>                      this_type;

   public:
   enum { type = IsListHook::type };

   typedef typename IsListHook::node_traits           node_traits;
   typedef typename IsListHook::node_ptr              node_ptr;
   typedef typename IsListHook::const_node_ptr        const_node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, this_type>::type                  this_type_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type>::type            const_this_type_ptr;

   public:
   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using ihashset_member_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_member_hook()
      :  m_islisthook()
   {}

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using ihashset_member_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_member_hook(const ihashset_member_hook &other)
      :  m_islisthook(other.m_islisthook)
   {}

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //!   The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using ihashset_member_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_member_hook& operator=(const ihashset_member_hook &other) 
   {  m_islisthook = other.m_islisthook;  return *this;  }

   //! <b>Effects</b>: If SafeMode is set to false, the destructor does
   //!   nothing (ie. no code is generated). If SafeMode is true and the
   //!   object is stored in an ihashset/ihashmultiset an assertion is raised.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~ihashset_member_hook() 
   {}

   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether ihashset/ihashmultiset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool linked() const 
   {  return m_islisthook.linked();  }

   //! The value_traits class is used as the first template argument for ihashset/ihashmultiset. 
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
   {  m_islisthook.unlink_self();   }

   //! <b>Effects</b>: Converts a pointer to a node stored in a sequence into
   //!   a pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static this_type_ptr to_hook_ptr(node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return this_type_ptr((this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook_ptr(const_node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return const_this_type_ptr((const this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   node_ptr to_node_ptr()
   {  return m_islisthook.to_node_ptr();  }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing. 
   const_node_ptr to_node_ptr() const
   {  return m_islisthook.to_node_ptr();  }
};

//! Put a public data member ihashset_auto_member_hook in order to store objects of this class in
//! an ihashset/ihashmultiset. ihashset_auto_member_hook holds the data necessary for maintaining the list and 
//! provides an appropriate value_traits class for ihashset/ihashmultiset.
//!
//! The difference between ihashset_auto_member_hook and ihashset_auto_member_hook is that
//! ihashset_auto_member_hook removes itself automatically from the container
//! in the assignment operator and the destructor. It also provides a new
//! "unlink_self" method so that the user can unlink its class without using
//! the container.
//!
//! ihashset_auto_member_hook can only be used with non constant-time ihashsets/ihashmultisets because
//! only those lists can support the auto-unlink feature.
//! 
//! The first template argument T defines the class type stored in ihashset/ihashmultiset. Objects of
//! type T and of types derived from T can be stored. T doesn't need to be 
//! copy-constructible or assignable.
//!
//! The second argument is the pointer type that will be used internally in the hook
//! and the ihashset/ihashmultiset configured from this hook.
template<class T, class VoidPointer = void*>
class ihashset_auto_member_hook
{
   typedef islist_auto_member_hook<T, VoidPointer> IsListHook;
   IsListHook m_islisthook;
   typedef ihashset_auto_member_hook
      <T, VoidPointer>                                this_type;

   public:
   enum { type = IsListHook::type };

   typedef typename IsListHook::node_traits           node_traits;
   typedef typename IsListHook::node_ptr              node_ptr;
   typedef typename IsListHook::const_node_ptr        const_node_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, this_type>::type                  this_type_ptr;
   typedef typename detail::pointer_to_other
      <VoidPointer, const this_type>::type            const_this_type_ptr;

   public:

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a default-constructor
   //!   makes classes using islist_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_auto_member_hook()
      :  m_islisthook()
   {}

   //! <b>Effects</b>: Initializes the node, such that it is not 
   //!   in a sequence. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using islist_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_auto_member_hook(const ihashset_auto_member_hook &other)
      :  m_islisthook(other.m_islisthook)
   {}

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //!   The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using islist_auto_base_hook STL-compliant without forcing the 
   //!   user to do some additional work.
   ihashset_auto_member_hook& operator=(const ihashset_auto_member_hook &other)
   {  m_islisthook = other.m_islisthook;  return *this;  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~ihashset_auto_member_hook() 
   {}

   //! <b>Returns</b>: true, if the node belongs to a sequence, false
   //!   otherwise. This function can be used to test whether ihashset/ihashmultiset::current 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool linked() const 
   {  return m_islisthook.linked();  }

   //! <b>Effects</b>: Removes the node if it's inserted in a sequence.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink_self()
   {  return m_islisthook.unlink_self();  }

   //! The value_traits class is used as the first template argument for ihashset/ihashmultiset. 
   //! The template argument is a pointer to member pointing to the node in 
   //! the class. Objects of type T and of types derived from T can be stored. 
   //! T doesn't need to be copy-constructible or assignable.
   template<this_type T::* M>
   struct value_traits
      : detail::member_value_traits<T, this_type, M>
   {};

   //! <b>Effects</b>: Converts a pointer to a node stored in a sequence into
   //!   a pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static this_type_ptr to_hook_ptr(node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return this_type_ptr((this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Converts a const pointer to a node stored in a sequence into
   //!   a const pointer to the hook that holds that node.
   //! 
   //! <b>Throws</b>: Nothing. 
   static const_this_type_ptr to_hook_ptr(const_node_ptr p)
   {
      //BOOST_ASSERT((const char*)((const this_type*)0) == (const char*)&((const this_type*)0)->m_islisthook);
      using boost::get_pointer;
      return const_this_type_ptr((const this_type*)get_pointer(IsListHook::to_hook_ptr(p)));
   }

   //! <b>Effects</b>: Returns a pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing.
   node_ptr to_node_ptr()
   {  return m_islisthook.to_node_ptr();  }

   //! <b>Effects</b>: Returns a const pointer to the node that this hook holds.
   //! 
   //! <b>Throws</b>: Nothing.
   const_node_ptr to_node_ptr() const
   {  return m_islisthook.to_node_ptr();  }
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_IHASHSET_HOOK_HPP
