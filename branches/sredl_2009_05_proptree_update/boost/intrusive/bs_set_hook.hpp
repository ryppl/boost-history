/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2007-2008
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_BS_SET_HOOK_HPP
#define BOOST_INTRUSIVE_BS_SET_HOOK_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/detail/tree_node.hpp>
#include <boost/intrusive/detail/tree_algorithms.hpp>
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/detail/generic_hook.hpp>

namespace boost {
namespace intrusive {

/// @cond
template<class VoidPointer>
struct get_bs_set_node_algo
{
   typedef detail::tree_algorithms<tree_node_traits<VoidPointer> > type;
};
/// @endcond

//! Helper metafunction to define a \c bs_set_base_hook that yields to the same
//! type when the same options (either explicitly or implicitly) are used.
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class ...Options>
#else
template<class O1 = none, class O2 = none, class O3 = none>
#endif
struct make_bs_set_base_hook
{
   /// @cond
   typedef typename pack_options
   #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      < hook_defaults, O1, O2, O3>
   #else
      < hook_defaults, Options...>
   #endif
   ::type packed_options;

   //Scapegoat trees can't be auto unlink trees
   BOOST_STATIC_ASSERT(((int)packed_options::link_mode != (int)auto_unlink));

   typedef detail::generic_hook
   < get_bs_set_node_algo<typename packed_options::void_pointer>
   , typename packed_options::tag
   , packed_options::link_mode
   , detail::BsSetBaseHook
   > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};

//! Derive a class from bs_set_base_hook in order to store objects in 
//! in a bs_set/bs_multiset. bs_set_base_hook holds the data necessary to maintain 
//! the bs_set/bs_multiset and provides an appropriate value_traits class for bs_set/bs_multiset.
//! 
//! The hook admits the following options: \c tag<>, \c void_pointer<>,
//! \c link_mode<>.
//!
//! \c tag<> defines a tag to identify the node. 
//! The same tag value can be used in different classes, but if a class is 
//! derived from more than one \c list_base_hook, then each \c list_base_hook needs its 
//! unique tag.
//!
//! \c void_pointer<> is the pointer type that will be used internally in the hook
//! and the the container configured to use this hook.
//!
//! \c link_mode<> will specify the linking mode of the hook (\c normal_link,
//! \c auto_unlink or \c safe_link).
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class ...Options>
#else
template<class O1, class O2, class O3>
#endif
class bs_set_base_hook
   :  public make_bs_set_base_hook
   #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      <O1, O2, O3>
   #else
      <Options...>
   #endif
   ::type

{
   #if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state.
   //! 
   //! <b>Throws</b>: Nothing. 
   bs_set_base_hook();

   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   bs_set_base_hook(const bs_set_base_hook& );

   //! <b>Effects</b>: Empty function. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   bs_set_base_hook& operator=(const bs_set_base_hook& );

   //! <b>Effects</b>: If link_mode is \c normal_link, the destructor does
   //!   nothing (ie. no code is generated). If link_mode is \c safe_link and the
   //!   object is stored in a set an assertion is raised. If link_mode is
   //!   \c auto_unlink and \c is_linked() is true, the node is unlinked.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~bs_set_base_hook();

   //! <b>Effects</b>: Swapping two nodes swaps the position of the elements 
   //!   related to those nodes in one or two containers. That is, if the node 
   //!   this is part of the element e1, the node x is part of the element e2 
   //!   and both elements are included in the containers s1 and s2, then after 
   //!   the swap-operation e1 is in s2 at the position of e2 and e2 is in s1 
   //!   at the position of e1. If one element is not in a container, then 
   //!   after the swap-operation the other element is not in a container. 
   //!   Iterators to e1 and e2 related to those nodes are invalidated. 
   //!
   //! <b>Complexity</b>: Constant 
   //!
   //! <b>Throws</b>: Nothing. 
   void swap_nodes(bs_set_base_hook &other);

   //! <b>Precondition</b>: link_mode must be \c safe_link or \c auto_unlink.
   //!
   //! <b>Returns</b>: true, if the node belongs to a container, false
   //!   otherwise. This function can be used to test whether \c set::iterator_to 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool is_linked() const;

   //! <b>Effects</b>: Removes the node if it's inserted in a container.
   //!   This function is only allowed if link_mode is \c auto_unlink.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink();
   #endif
};

//! Helper metafunction to define a \c bs_set_member_hook that yields to the same
//! type when the same options (either explicitly or implicitly) are used.
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class ...Options>
#else
template<class O1 = none, class O2 = none, class O3 = none>
#endif
struct make_bs_set_member_hook
{
   /// @cond
   typedef typename pack_options
   #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      < hook_defaults, O1, O2, O3>
   #else
      < hook_defaults, Options...>
   #endif

   ::type packed_options;

   //Scapegoat trees can't be auto unlink trees
   BOOST_STATIC_ASSERT(((int)packed_options::link_mode != (int)auto_unlink));

   typedef detail::generic_hook
   < get_bs_set_node_algo<typename packed_options::void_pointer>
   , member_tag
   , packed_options::link_mode
   , detail::NoBaseHook
   > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};

//! Put a public data member bs_set_member_hook in order to store objects of this class in
//! a bs_set/bs_multiset. bs_set_member_hook holds the data necessary for maintaining the
//! bs_set/bs_multiset and provides an appropriate value_traits class for bs_set/bs_multiset.
//! 
//! The hook admits the following options: \c void_pointer<>, \c link_mode<>.
//!
//! \c void_pointer<> is the pointer type that will be used internally in the hook
//! and the the container configured to use this hook.
//!
//! \c link_mode<> will specify the linking mode of the hook (\c normal_link,
//! \c auto_unlink or \c safe_link).
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class ...Options>
#else
template<class O1, class O2, class O3>
#endif
class bs_set_member_hook
   :  public make_bs_set_member_hook
      #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      <O1, O2, O3>
      #else
      <Options...>
      #endif
      ::type
{
   #if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state.
   //! 
   //! <b>Throws</b>: Nothing. 
   bs_set_member_hook();

   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   bs_set_member_hook(const bs_set_member_hook& );

   //! <b>Effects</b>: Empty function. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   bs_set_member_hook& operator=(const bs_set_member_hook& );

   //! <b>Effects</b>: If link_mode is \c normal_link, the destructor does
   //!   nothing (ie. no code is generated). If link_mode is \c safe_link and the
   //!   object is stored in a set an assertion is raised. If link_mode is
   //!   \c auto_unlink and \c is_linked() is true, the node is unlinked.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~bs_set_member_hook();

   //! <b>Effects</b>: Swapping two nodes swaps the position of the elements 
   //!   related to those nodes in one or two containers. That is, if the node 
   //!   this is part of the element e1, the node x is part of the element e2 
   //!   and both elements are included in the containers s1 and s2, then after 
   //!   the swap-operation e1 is in s2 at the position of e2 and e2 is in s1 
   //!   at the position of e1. If one element is not in a container, then 
   //!   after the swap-operation the other element is not in a container. 
   //!   Iterators to e1 and e2 related to those nodes are invalidated. 
   //!
   //! <b>Complexity</b>: Constant 
   //!
   //! <b>Throws</b>: Nothing. 
   void swap_nodes(bs_set_member_hook &other);

   //! <b>Precondition</b>: link_mode must be \c safe_link or \c auto_unlink.
   //!
   //! <b>Returns</b>: true, if the node belongs to a container, false
   //!   otherwise. This function can be used to test whether \c set::iterator_to 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool is_linked() const;

   //! <b>Effects</b>: Removes the node if it's inserted in a container.
   //!   This function is only allowed if link_mode is \c auto_unlink.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink();
   #endif
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_BS_SET_HOOK_HPP
