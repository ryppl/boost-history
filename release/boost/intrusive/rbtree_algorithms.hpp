/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2007.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
// The internal implementation of red-black trees is based on that of SGI STL
// stl_tree.h file: 
//
// Copyright (c) 1996,1997
// Silicon Graphics Computer Systems, Inc.
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Silicon Graphics makes no
// representations about the suitability of this software for any
// purpose.  It is provided "as is" without express or implied warranty.
//
//
// Copyright (c) 1994
// Hewlett-Packard Company
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hewlett-Packard Company makes no
// representations about the suitability of this software for any
// purpose.  It is provided "as is" without express or implied warranty.
//
// The tree destruction algorithm is based on Julienne Walker and The EC Team code: 
// 
// This code is in the public domain. Anyone may use it or change it in any way that
// they see fit. The author assumes no responsibility for damages incurred through
// use of the original code or any variations thereof. 
// 
// It is requested, but not required, that due credit is given to the original author
// and anyone who has modified the code through a header comment, such as this one. 

#ifndef BOOST_INTRUSIVE_RBTREE_ALGORITHMS_HPP
#define BOOST_INTRUSIVE_RBTREE_ALGORITHMS_HPP

#include <boost/intrusive/detail/config_begin.hpp>

#include <cstddef>
#include <boost/intrusive/intrusive_fwd.hpp>

#include <boost/intrusive/detail/assert.hpp>
#include <boost/intrusive/detail/no_exceptions_support.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/detail/tree_algorithms.hpp>


namespace boost {
namespace intrusive {

//! rbtree_algorithms provides basic algorithms to manipulate 
//! nodes forming a red-black tree. The insertion and deletion algorithms are 
//! based on those in Cormen, Leiserson, and Rivest, Introduction to Algorithms 
//! (MIT Press, 1990), except that
//! 
//! (1) the header node is maintained with links not only to the root
//! but also to the leftmost node of the tree, to enable constant time
//! begin(), and to the rightmost node of the tree, to enable linear time
//! performance when used with the generic set algorithms (set_union,
//! etc.);
//! 
//! (2) when a node being deleted has two children its successor node is
//! relinked into its place, rather than copied, so that the only
//! pointers invalidated are those referring to the deleted node.
//!
//! rbtree_algorithms is configured with a NodeTraits class, which encapsulates the
//! information about the node to be manipulated. NodeTraits must support the
//! following interface:
//!
//! <b>Typedefs</b>:
//!
//! <tt>node</tt>: The type of the node that forms the circular list
//!
//! <tt>node_ptr</tt>: A pointer to a node
//!
//! <tt>const_node_ptr</tt>: A pointer to a const node
//!
//! <tt>color</tt>: The type that can store the color of a node
//!
//! <b>Static functions</b>:
//!
//! <tt>static node_ptr get_parent(const_node_ptr n);</tt>
//! 
//! <tt>static void set_parent(node_ptr n, node_ptr parent);</tt>
//!
//! <tt>static node_ptr get_left(const_node_ptr n);</tt>
//! 
//! <tt>static void set_left(node_ptr n, node_ptr left);</tt>
//!
//! <tt>static node_ptr get_right(const_node_ptr n);</tt>
//! 
//! <tt>static void set_right(node_ptr n, node_ptr right);</tt>
//! 
//! <tt>static color get_color(const_node_ptr n);</tt>
//! 
//! <tt>static void set_color(node_ptr n, color c);</tt>
//! 
//! <tt>static color black();</tt>
//! 
//! <tt>static color red();</tt>
template<class NodeTraits>
class rbtree_algorithms
{
   public:
   typedef NodeTraits                           node_traits;
   typedef typename NodeTraits::node_ptr        node_ptr;
   typedef typename NodeTraits::const_node_ptr  const_node_ptr;
   typedef typename NodeTraits::color           color;

   /// @cond
   private:

   typedef typename NodeTraits::node            node;
   typedef detail::tree_algorithms<NodeTraits>  tree_algorithms;

   template<class F>
   struct rbtree_node_cloner
      :  private detail::ebo_functor_holder<F>
   {
      typedef detail::ebo_functor_holder<F>                 base_t;

      rbtree_node_cloner(F f)
         :  base_t(f)
      {}
      
      node_ptr operator()(node_ptr p)
      {
         node_ptr n = base_t::get()(p);
         NodeTraits::set_color(n, NodeTraits::get_color(p));
         return n;
      }
   };

   struct rbtree_erase_fixup
   {
      void operator()(node_ptr to_erase, node_ptr successor)
      {
         //Swap color of y and z
         color tmp(NodeTraits::get_color(successor));
         NodeTraits::set_color(successor, NodeTraits::get_color(to_erase));
         NodeTraits::set_color(to_erase, tmp);
      }
   };

   static node_ptr uncast(const_node_ptr ptr)
   {
      return node_ptr(const_cast<node*>(::boost::intrusive::detail::get_pointer(ptr)));
   }
   /// @endcond

   public:
   static node_ptr begin_node(const_node_ptr header)
   {  return tree_algorithms::begin_node(header);   }

   static node_ptr end_node(const_node_ptr header)
   {  return tree_algorithms::end_node(header);   }

   //! This type is the information that will be
   //! filled by insert_unique_check
   typedef typename tree_algorithms::insert_commit_data insert_commit_data;

   //! <b>Requires</b>: header1 and header2 must be the header nodes
   //!  of two trees.
   //! 
   //! <b>Effects</b>: Swaps two trees. After the function header1 will contain 
   //!   links to the second tree and header2 will have links to the first tree.
   //! 
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing.
   static void swap_tree(node_ptr header1, node_ptr header2)
   {  return tree_algorithms::swap_tree(header1, header2);  }

   //! <b>Requires</b>: node1 and node2 can't be header nodes
   //!  of two trees.
   //! 
   //! <b>Effects</b>: Swaps two nodes. After the function node1 will be inserted
   //!   in the position node2 before the function. node2 will be inserted in the
   //!   position node1 had before the function.
   //! 
   //! <b>Complexity</b>: Logarithmic. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function will break container ordering invariants if
   //!   node1 and node2 are not equivalent according to the ordering rules.
   //!
   //!Experimental function
   static void swap_nodes(node_ptr node1, node_ptr node2)
   {
      if(node1 == node2)
         return;
   
      node_ptr header1(tree_algorithms::get_header(node1)), header2(tree_algorithms::get_header(node2));
      swap_nodes(node1, header1, node2, header2);
   }

   //! <b>Requires</b>: node1 and node2 can't be header nodes
   //!  of two trees with header header1 and header2.
   //! 
   //! <b>Effects</b>: Swaps two nodes. After the function node1 will be inserted
   //!   in the position node2 before the function. node2 will be inserted in the
   //!   position node1 had before the function.
   //! 
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function will break container ordering invariants if
   //!   node1 and node2 are not equivalent according to the ordering rules.
   //!
   //!Experimental function
   static void swap_nodes(node_ptr node1, node_ptr header1, node_ptr node2, node_ptr header2)
   {
      if(node1 == node2)   return;

      tree_algorithms::swap_nodes(node1, header1, node2, header2);
      //Swap color
      color c = NodeTraits::get_color(node1);
      NodeTraits::set_color(node1, NodeTraits::get_color(node2)); 
      NodeTraits::set_color(node2, c); 
   }

   //! <b>Requires</b>: node_to_be_replaced must be inserted in a tree
   //!   and new_node must not be inserted in a tree.
   //! 
   //! <b>Effects</b>: Replaces node_to_be_replaced in its position in the
   //!   tree with new_node. The tree does not need to be rebalanced
   //! 
   //! <b>Complexity</b>: Logarithmic. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function will break container ordering invariants if
   //!   new_node is not equivalent to node_to_be_replaced according to the
   //!   ordering rules. This function is faster than erasing and inserting
   //!   the node, since no rebalancing and comparison is needed.
   //!
   //!Experimental function
   static void replace_node(node_ptr node_to_be_replaced, node_ptr new_node)
   {
      if(node_to_be_replaced == new_node)
         return;
      replace_node(node_to_be_replaced, tree_algorithms::get_header(node_to_be_replaced), new_node);
   }

   //! <b>Requires</b>: node_to_be_replaced must be inserted in a tree
   //!   with header "header" and new_node must not be inserted in a tree.
   //! 
   //! <b>Effects</b>: Replaces node_to_be_replaced in its position in the
   //!   tree with new_node. The tree does not need to be rebalanced
   //! 
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function will break container ordering invariants if
   //!   new_node is not equivalent to node_to_be_replaced according to the
   //!   ordering rules. This function is faster than erasing and inserting
   //!   the node, since no rebalancing or comparison is needed.
   //!
   //!Experimental function
   static void replace_node(node_ptr node_to_be_replaced, node_ptr header, node_ptr new_node)
   {
      tree_algorithms::replace_node(node_to_be_replaced, header, new_node);
      NodeTraits::set_color(new_node, NodeTraits::get_color(node_to_be_replaced)); 
   }

   //! <b>Requires</b>: node is a tree node but not the header.
   //! 
   //! <b>Effects</b>: Unlinks the node and rebalances the tree.
   //! 
   //! <b>Complexity</b>: Average complexity is constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static void unlink(node_ptr node)
   {
      node_ptr x = NodeTraits::get_parent(node);
      if(x){
         while(!is_header(x))
            x = NodeTraits::get_parent(x);
         erase(x, node);
      }
   }

   //! <b>Requires</b>: header is the header of a tree.
   //! 
   //! <b>Effects</b>: Unlinks the leftmost node from the tree, and
   //!   updates the header link to the new leftmost node.
   //! 
   //! <b>Complexity</b>: Average complexity is constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Notes</b>: This function breaks the tree and the tree can
   //!   only be used for more unlink_leftmost_without_rebalance calls.
   //!   This function is normally used to achieve a step by step
   //!   controlled destruction of the tree.
   static node_ptr unlink_leftmost_without_rebalance(node_ptr header)
   {  return tree_algorithms::unlink_leftmost_without_rebalance(header);   }

   //! <b>Requires</b>: node is a node of the tree or an node initialized
   //!   by init(...).
   //! 
   //! <b>Effects</b>: Returns true if the node is initialized by init().
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static bool unique(const_node_ptr node)
   {  return tree_algorithms::unique(node);  }

   //! <b>Requires</b>: node is a node of the tree but it's not the header.
   //! 
   //! <b>Effects</b>: Returns the number of nodes of the subtree.
   //! 
   //! <b>Complexity</b>: Linear time.
   //! 
   //! <b>Throws</b>: Nothing.
   static std::size_t count(const_node_ptr node)
   {  return tree_algorithms::count(node);   }

   //! <b>Requires</b>: header is the header node of the tree.
   //! 
   //! <b>Effects</b>: Returns the number of nodes above the header.
   //! 
   //! <b>Complexity</b>: Linear time.
   //! 
   //! <b>Throws</b>: Nothing.
   static std::size_t size(const_node_ptr header)
   {  return tree_algorithms::size(header);   }

   //! <b>Requires</b>: p is a node from the tree except the header.
   //! 
   //! <b>Effects</b>: Returns the next node of the tree.
   //! 
   //! <b>Complexity</b>: Average constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static node_ptr next_node(node_ptr p)
   {  return tree_algorithms::next_node(p); }

   //! <b>Requires</b>: p is a node from the tree except the leftmost node.
   //! 
   //! <b>Effects</b>: Returns the previous node of the tree.
   //! 
   //! <b>Complexity</b>: Average constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static node_ptr prev_node(node_ptr p)
   {  return tree_algorithms::prev_node(p); }

   //! <b>Requires</b>: node must not be part of any tree.
   //!
   //! <b>Effects</b>: After the function unique(node) == true.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Nodes</b>: If node is inserted in a tree, this function corrupts the tree.
   static void init(node_ptr node)
   {  tree_algorithms::init(node);  }

   //! <b>Requires</b>: node must not be part of any tree.
   //!
   //! <b>Effects</b>: Initializes the header to represent an empty tree.
   //!   unique(header) == true.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Nodes</b>: If node is inserted in a tree, this function corrupts the tree.
   static void init_header(node_ptr header)
   {
      tree_algorithms::init_header(header);
      NodeTraits::set_color(header, NodeTraits::red()); 
   }

   //! <b>Requires</b>: header must be the header of a tree, z a node
   //!    of that tree and z != header.
   //!
   //! <b>Effects</b>: Erases node "z" from the tree with header "header".
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static node_ptr erase(node_ptr header, node_ptr z)
   {
      typename tree_algorithms::data_for_rebalance info;
      tree_algorithms::erase(header, z, rbtree_erase_fixup(), info);
      node_ptr x = info.x;
      node_ptr x_parent = info.x_parent;

      //Rebalance rbtree
      if(NodeTraits::get_color(z) != NodeTraits::red()){
         rebalance_after_erasure(header, x, x_parent);
      }
      return z;
   }

   //! <b>Requires</b>: "cloner" must be a function
   //!   object taking a node_ptr and returning a new cloned node of it. "disposer" must
   //!   take a node_ptr and shouldn't throw.
   //!
   //! <b>Effects</b>: First empties target tree calling 
   //!   <tt>void disposer::operator()(node_ptr)</tt> for every node of the tree
   //!    except the header.
   //!    
   //!   Then, duplicates the entire tree pointed by "source_header" cloning each
   //!   source node with <tt>node_ptr Cloner::operator()(node_ptr)</tt> to obtain 
   //!   the nodes of the target tree. If "cloner" throws, the cloned target nodes
   //!   are disposed using <tt>void disposer(node_ptr)</tt>.
   //! 
   //! <b>Complexity</b>: Linear to the number of element of the source tree plus the.
   //!   number of elements of tree target tree when calling this function.
   //! 
   //! <b>Throws</b>: If cloner functor throws. If this happens target nodes are disposed.
   template <class Cloner, class Disposer>
   static void clone
      (const_node_ptr source_header, node_ptr target_header, Cloner cloner, Disposer disposer)
   {
      rbtree_node_cloner<Cloner> new_cloner(cloner);
      tree_algorithms::clone(source_header, target_header, new_cloner, disposer);
   }

   //! <b>Requires</b>: "disposer" must be an object function
   //!   taking a node_ptr parameter and shouldn't throw.
   //!
   //! <b>Effects</b>: Empties the target tree calling 
   //!   <tt>void disposer::operator()(node_ptr)</tt> for every node of the tree
   //!    except the header.
   //! 
   //! <b>Complexity</b>: Linear to the number of element of the source tree plus the.
   //!   number of elements of tree target tree when calling this function.
   //! 
   //! <b>Throws</b>: If cloner functor throws. If this happens target nodes are disposed.
   template<class Disposer>
   static void clear_and_dispose(node_ptr header, Disposer disposer)
   {  tree_algorithms::clear_and_dispose(header, disposer); }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   KeyNodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. KeyNodePtrCompare can compare KeyType with tree's node_ptrs.
   //!
   //! <b>Effects</b>: Returns an node_ptr to the first element that is
   //!   not less than "key" according to "comp" or "header" if that element does
   //!   not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class KeyType, class KeyNodePtrCompare>
   static node_ptr lower_bound
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
   {  return tree_algorithms::lower_bound(header, key, comp);  }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   KeyNodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. KeyNodePtrCompare can compare KeyType with tree's node_ptrs.
   //!
   //! <b>Effects</b>: Returns an node_ptr to the first element that is greater
   //!   than "key" according to "comp" or "header" if that element does not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class KeyType, class KeyNodePtrCompare>
   static node_ptr upper_bound
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
   {  return tree_algorithms::upper_bound(header, key, comp);  }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   KeyNodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. KeyNodePtrCompare can compare KeyType with tree's node_ptrs.
   //!
   //! <b>Effects</b>: Returns an node_ptr to the element that is equivalent to
   //!   "key" according to "comp" or "header" if that element does not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class KeyType, class KeyNodePtrCompare>
   static node_ptr find
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
   {  return tree_algorithms::find(header, key, comp);  }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   KeyNodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. KeyNodePtrCompare can compare KeyType with tree's node_ptrs.
   //!
   //! <b>Effects</b>: Returns an a pair of node_ptr delimiting a range containing
   //!   all elements that are equivalent to "key" according to "comp" or an
   //!   empty range that indicates the position where those elements would be
   //!   if they there are no equivalent elements.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, node_ptr> equal_range
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
   {  return tree_algorithms::equal_range(header, key, comp);  }

   //! <b>Requires</b>: "h" must be the header node of a tree.
   //!   NodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. NodePtrCompare compares two node_ptrs.
   //!
   //! <b>Effects</b>: Inserts new_node into the tree before the upper bound
   //!   according to "comp".
   //! 
   //! <b>Complexity</b>: Average complexity for insert element is at
   //!   most logarithmic.
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class NodePtrCompare>
   static node_ptr insert_equal_upper_bound
      (node_ptr h, node_ptr new_node, NodePtrCompare comp)
   {
      tree_algorithms::insert_equal_upper_bound(h, new_node, comp);
      rebalance_after_insertion(h, new_node);
      return new_node;
   }

   //! <b>Requires</b>: "h" must be the header node of a tree.
   //!   NodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. NodePtrCompare compares two node_ptrs.
   //!
   //! <b>Effects</b>: Inserts new_node into the tree before the lower bound
   //!   according to "comp".
   //! 
   //! <b>Complexity</b>: Average complexity for insert element is at
   //!   most logarithmic.
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class NodePtrCompare>
   static node_ptr insert_equal_lower_bound
      (node_ptr h, node_ptr new_node, NodePtrCompare comp)
   {
      tree_algorithms::insert_equal_lower_bound(h, new_node, comp);
      rebalance_after_insertion(h, new_node);
      return new_node;
   }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   NodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. NodePtrCompare compares two node_ptrs. "hint" is node from
   //!   the "header"'s tree.
   //!   
   //! <b>Effects</b>: Inserts new_node into the tree, using "hint" as a hint to
   //!   where it will be inserted. If "hint" is the upper_bound
   //!   the insertion takes constant time (two comparisons in the worst case).
   //!
   //! <b>Complexity</b>: Logarithmic in general, but it is amortized
   //!   constant time if new_node is inserted immediately before "hint".
   //! 
   //! <b>Throws</b>: If "comp" throws.
   template<class NodePtrCompare>
   static node_ptr insert_equal
      (node_ptr header, node_ptr hint, node_ptr new_node, NodePtrCompare comp)
   {
      tree_algorithms::insert_equal(header, hint, new_node, comp);
      rebalance_after_insertion(header, new_node);
      return new_node;
   }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   KeyNodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. NodePtrCompare compares KeyType with a node_ptr.
   //! 
   //! <b>Effects</b>: Checks if there is an equivalent node to "key" in the
   //!   tree according to "comp" and obtains the needed information to realize
   //!   a constant-time node insertion if there is no equivalent node.
   //!
   //! <b>Returns</b>: If there is an equivalent value
   //!   returns a pair containing a node_ptr to the already present node
   //!   and false. If there is not equivalent key can be inserted returns true
   //!   in the returned pair's boolean and fills "commit_data" that is meant to
   //!   be used with the "insert_commit" function to achieve a constant-time
   //!   insertion function.
   //! 
   //! <b>Complexity</b>: Average complexity is at most logarithmic.
   //!
   //! <b>Throws</b>: If "comp" throws.
   //! 
   //! <b>Notes</b>: This function is used to improve performance when constructing
   //!   a node is expensive and the user does not want to have two equivalent nodes
   //!   in the tree: if there is an equivalent value
   //!   the constructed object must be discarded. Many times, the part of the
   //!   node that is used to impose the order is much cheaper to construct
   //!   than the node and this function offers the possibility to use that part
   //!   to check if the insertion will be successful.
   //!
   //!   If the check is successful, the user can construct the node and use
   //!   "insert_commit" to insert the node in constant-time. This gives a total
   //!   logarithmic complexity to the insertion: check(O(log(N)) + commit(O(1)).
   //!
   //!   "commit_data" remains valid for a subsequent "insert_unique_commit" only
   //!   if no more objects are inserted or erased from the set.
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, bool> insert_unique_check
      (const_node_ptr header,  const KeyType &key
      ,KeyNodePtrCompare comp, insert_commit_data &commit_data)
   {  return tree_algorithms::insert_unique_check(header, key, comp, commit_data);  }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   KeyNodePtrCompare is a function object that induces a strict weak
   //!   ordering compatible with the strict weak ordering used to create the
   //!   the tree. NodePtrCompare compares KeyType with a node_ptr.
   //!   "hint" is node from the "header"'s tree.
   //! 
   //! <b>Effects</b>: Checks if there is an equivalent node to "key" in the
   //!   tree according to "comp" using "hint" as a hint to where it should be
   //!   inserted and obtains the needed information to realize
   //!   a constant-time node insertion if there is no equivalent node. 
   //!   If "hint" is the upper_bound the function has constant time 
   //!   complexity (two comparisons in the worst case).
   //!
   //! <b>Returns</b>: If there is an equivalent value
   //!   returns a pair containing a node_ptr to the already present node
   //!   and false. If there is not equivalent key can be inserted returns true
   //!   in the returned pair's boolean and fills "commit_data" that is meant to
   //!   be used with the "insert_commit" function to achieve a constant-time
   //!   insertion function.
   //! 
   //! <b>Complexity</b>: Average complexity is at most logarithmic, but it is
   //!   amortized constant time if new_node should be inserted immediately before "hint".
   //!
   //! <b>Throws</b>: If "comp" throws.
   //! 
   //! <b>Notes</b>: This function is used to improve performance when constructing
   //!   a node is expensive and the user does not want to have two equivalent nodes
   //!   in the tree: if there is an equivalent value
   //!   the constructed object must be discarded. Many times, the part of the
   //!   node that is used to impose the order is much cheaper to construct
   //!   than the node and this function offers the possibility to use that part
   //!   to check if the insertion will be successful.
   //!
   //!   If the check is successful, the user can construct the node and use
   //!   "insert_commit" to insert the node in constant-time. This gives a total
   //!   logarithmic complexity to the insertion: check(O(log(N)) + commit(O(1)).
   //!
   //!   "commit_data" remains valid for a subsequent "insert_unique_commit" only
   //!   if no more objects are inserted or erased from the set.
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, bool> insert_unique_check
      (const_node_ptr header,  node_ptr hint, const KeyType &key
      ,KeyNodePtrCompare comp, insert_commit_data &commit_data)
   {  return tree_algorithms::insert_unique_check(header, hint, key, comp, commit_data);  }

   //! <b>Requires</b>: "header" must be the header node of a tree.
   //!   "commit_data" must have been obtained from a previous call to
   //!   "insert_unique_check". No objects should have been inserted or erased
   //!   from the set between the "insert_unique_check" that filled "commit_data"
   //!   and the call to "insert_commit". 
   //! 
   //! 
   //! <b>Effects</b>: Inserts new_node in the set using the information obtained
   //!   from the "commit_data" that a previous "insert_check" filled.
   //!
   //! <b>Complexity</b>: Constant time.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Notes</b>: This function has only sense if a "insert_unique_check" has been
   //!   previously executed to fill "commit_data". No value should be inserted or
   //!   erased between the "insert_check" and "insert_commit" calls.
   static void insert_unique_commit
      (node_ptr header, node_ptr new_value, const insert_commit_data &commit_data)
   {
      tree_algorithms::insert_unique_commit(header, new_value, commit_data);
      rebalance_after_insertion(header, new_value);
   }

   /// @cond
   private:

   //! <b>Requires</b>: p is a node of a tree.
   //! 
   //! <b>Effects</b>: Returns true if p is the header of the tree.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static bool is_header(const_node_ptr p)
   {
      return NodeTraits::get_color(p) == NodeTraits::red() &&
            tree_algorithms::is_header(p);
      //return NodeTraits::get_color(p) == NodeTraits::red() && 
      //       NodeTraits::get_parent(NodeTraits::get_parent(p)) == p;
   }

   static void rebalance_after_erasure(node_ptr header, node_ptr x, node_ptr x_parent)
   {
      while(x != NodeTraits::get_parent(header) && (x == 0 || NodeTraits::get_color(x) == NodeTraits::black())){
         if(x == NodeTraits::get_left(x_parent)){
            node_ptr w = NodeTraits::get_right(x_parent);
            if(NodeTraits::get_color(w) == NodeTraits::red()){
               NodeTraits::set_color(w, NodeTraits::black());
               NodeTraits::set_color(x_parent, NodeTraits::red());
               tree_algorithms::rotate_left(x_parent, header);
               w = NodeTraits::get_right(x_parent);
            }
            if((NodeTraits::get_left(w) == 0 || NodeTraits::get_color(NodeTraits::get_left(w))  == NodeTraits::black()) &&
               (NodeTraits::get_right(w) == 0 || NodeTraits::get_color(NodeTraits::get_right(w)) == NodeTraits::black())){
               NodeTraits::set_color(w, NodeTraits::red());
               x = x_parent;
               x_parent = NodeTraits::get_parent(x_parent);
            } 
            else {
               if(NodeTraits::get_right(w) == 0 || NodeTraits::get_color(NodeTraits::get_right(w)) == NodeTraits::black()){
                  NodeTraits::set_color(NodeTraits::get_left(w), NodeTraits::black());
                  NodeTraits::set_color(w, NodeTraits::red());
                  tree_algorithms::rotate_right(w, header);
                  w = NodeTraits::get_right(x_parent);
               }
               NodeTraits::set_color(w, NodeTraits::get_color(x_parent));
               NodeTraits::set_color(x_parent, NodeTraits::black());
               if(NodeTraits::get_right(w))
                  NodeTraits::set_color(NodeTraits::get_right(w), NodeTraits::black());
               tree_algorithms::rotate_left(x_parent, header);
               break;
            }
         }
         else {
            // same as above, with right_ <-> left_.
            node_ptr w = NodeTraits::get_left(x_parent);
            if(NodeTraits::get_color(w) == NodeTraits::red()){
               NodeTraits::set_color(w, NodeTraits::black());
               NodeTraits::set_color(x_parent, NodeTraits::red());
               tree_algorithms::rotate_right(x_parent, header);
               w = NodeTraits::get_left(x_parent);
            }
            if((NodeTraits::get_right(w) == 0 || NodeTraits::get_color(NodeTraits::get_right(w)) == NodeTraits::black()) &&
               (NodeTraits::get_left(w) == 0 || NodeTraits::get_color(NodeTraits::get_left(w)) == NodeTraits::black())){
               NodeTraits::set_color(w, NodeTraits::red());
               x = x_parent;
               x_parent = NodeTraits::get_parent(x_parent);
            }
            else {
               if(NodeTraits::get_left(w) == 0 || NodeTraits::get_color(NodeTraits::get_left(w)) == NodeTraits::black()){
                  NodeTraits::set_color(NodeTraits::get_right(w), NodeTraits::black());
                  NodeTraits::set_color(w, NodeTraits::red());
                  tree_algorithms::rotate_left(w, header);
                  w = NodeTraits::get_left(x_parent);
               }
               NodeTraits::set_color(w, NodeTraits::get_color(x_parent));
               NodeTraits::set_color(x_parent, NodeTraits::black());
               if(NodeTraits::get_left(w))
                  NodeTraits::set_color(NodeTraits::get_left(w), NodeTraits::black());
               tree_algorithms::rotate_right(x_parent, header);
               break;
            }
         }
      }
      if(x)
         NodeTraits::set_color(x, NodeTraits::black());
   }


   static void rebalance_after_insertion(node_ptr header, node_ptr p)
   {
      NodeTraits::set_color(p, NodeTraits::red());
      while(p != NodeTraits::get_parent(header) && NodeTraits::get_color(NodeTraits::get_parent(p)) == NodeTraits::red()){
         node_ptr p_parent(NodeTraits::get_parent(p));
         node_ptr p_parent_parent(NodeTraits::get_parent(p_parent));
         if(tree_algorithms::is_left_child(p_parent)){
            node_ptr x = NodeTraits::get_right(p_parent_parent);
            if(x && NodeTraits::get_color(x) == NodeTraits::red()){
               NodeTraits::set_color(p_parent, NodeTraits::black());
               NodeTraits::set_color(p_parent_parent, NodeTraits::red());
               NodeTraits::set_color(x, NodeTraits::black());
               p = p_parent_parent;
            }
            else {
               if(!tree_algorithms::is_left_child(p)){
                  p = p_parent;
                  tree_algorithms::rotate_left(p, header);
               }
               node_ptr new_p_parent(NodeTraits::get_parent(p));
               node_ptr new_p_parent_parent(NodeTraits::get_parent(new_p_parent));
               NodeTraits::set_color(new_p_parent, NodeTraits::black());
               NodeTraits::set_color(new_p_parent_parent, NodeTraits::red());
               tree_algorithms::rotate_right(new_p_parent_parent, header);
            }
         }
         else{
            node_ptr x = NodeTraits::get_left(p_parent_parent);
            if(x && NodeTraits::get_color(x) == NodeTraits::red()){
               NodeTraits::set_color(p_parent, NodeTraits::black());
               NodeTraits::set_color(p_parent_parent, NodeTraits::red());
               NodeTraits::set_color(x, NodeTraits::black());
               p = p_parent_parent;
            }
            else{
               if(tree_algorithms::is_left_child(p)){
                  p = p_parent;
                  tree_algorithms::rotate_right(p, header);
               }
               node_ptr new_p_parent(NodeTraits::get_parent(p));
               node_ptr new_p_parent_parent(NodeTraits::get_parent(new_p_parent));
               NodeTraits::set_color(new_p_parent, NodeTraits::black());
               NodeTraits::set_color(new_p_parent_parent, NodeTraits::red());
               tree_algorithms::rotate_left(new_p_parent_parent, header);
            }
         }
      }
      NodeTraits::set_color(NodeTraits::get_parent(header), NodeTraits::black());
   }
   /// @endcond
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_RBTREE_ALGORITHMS_HPP
