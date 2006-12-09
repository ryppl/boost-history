/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gazta�aga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_LIST_ALGORITHMS_HPP
#define BOOST_INTRUSIVE_LIST_ALGORITHMS_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/get_pointer.hpp>
#include <cstddef>

namespace boost {
namespace intrusive {

//! list_algorithms provides basic algorithms to manipulate a sequence of nodes
//! forming a circular doubly linked list. An empty sequence is formed by a node
//! whose pointer to the next and previous node point to itself.
//!
//! list_algorithms is configured with a NodeTraits class, which capsulates the
//! information about the node to be manipulated. NodeTraits must support the
//! following interface:
//!
//! <b>Typedefs</b>:
//!
//! <tt>node</tt>: The type of the node that forms the circular list
//!
//! <tt>void_pointer</tt>: The type of pointer of the node (usually void*)
//!
//! <b>Static functions</b>:
//!
//! <tt>static node_ptr get_previous(const_node_ptr n);</tt>
//! 
//! <tt>static void set_previous(node_ptr n, node_ptr prev);</tt>
//! 
//! <tt>static node_ptr get_next(const_node_ptr n);</tt>
//! 
//! <tt>static void set_next(node_ptr n, node_ptr next);</tt>
template<class NodeTraits>
struct list_algorithms
{
   //! A pointer to NodeTraits::node of the same type as NodeTraits::void_pointer
   typedef typename detail::pointer_to_other
      <typename NodeTraits::void_pointer
      ,typename NodeTraits::node>::type            node_ptr;

   //! A pointer to const NodeTraits::node of the same type as NodeTraits::void_pointer
   typedef typename detail::pointer_to_other
      <typename NodeTraits::void_pointer
      ,const typename NodeTraits::node>::type      const_node_ptr;

   //! <b>Effects</b>: Constructs an empty list, making this_node the only
   //!   node of the sequence:
   //!  <tt>NodeTraits::get_next(this_node) == NodeTraits::get_previous(this_node)
   //!  == this_node</tt>.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void init(node_ptr this_node)
   {
      NodeTraits::set_next(this_node, this_node);
      NodeTraits::set_previous(this_node, this_node);
   }  

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Returns true is "this_node" is the only node of a sequence:
   //!  <tt>return NodeTraits::get_next(this_node) == this_node</tt>
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static bool unique(const_node_ptr this_node)  
   {  return NodeTraits::get_next(this_node) == this_node;  }

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Returns the number of nodes in a sequence. If the sequence
   //!  is empty, returns 1.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static std::size_t count(const_node_ptr this_node) 
   {
      std::size_t result = 0;
      const_node_ptr p = this_node;
      do{
         p = NodeTraits::get_next(p);
         ++result;
      }while (p != this_node);
      return result;
   }

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Unlinks the node from the sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static node_ptr unlink(node_ptr this_node)
   {
      node_ptr next(NodeTraits::get_next(this_node));
      node_ptr prev(NodeTraits::get_previous(this_node));
      NodeTraits::set_next(prev, next);
      NodeTraits::set_previous(next, prev);
      return next;
   }

   //! <b>Requires</b>: b and e must be nodes of the same sequence or an empty range.
   //! 
   //! <b>Effects</b>: Unlinks the node range [b, e) from the sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void unlink(node_ptr b, node_ptr e)
   {
      if (b != e) {
         node_ptr prev(NodeTraits::get_previous(b));
         node_ptr next(NodeTraits::get_next(e));
         NodeTraits::set_previous(next, prev);
         NodeTraits::set_next(prev, next);
      }
   }

   //! <b>Requires</b>: nxt_node must be a node of a sequence.
   //! 
   //! <b>Effects</b>: Links this_node before nxt_node in the sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void link_before(node_ptr this_node, node_ptr nxt_node)
   {
      node_ptr prev(NodeTraits::get_previous(nxt_node));
      NodeTraits::set_previous(this_node, prev);
      NodeTraits::set_next(prev, this_node);
      NodeTraits::set_previous(nxt_node, this_node);
      NodeTraits::set_next(this_node, nxt_node);
   }

   //! <b>Requires</b>: prev_node must be a node of a sequence.
   //! 
   //! <b>Effects</b>: Links this_node after prev_node in the sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void link_after(node_ptr this_node, node_ptr prev_node)
   {
      node_ptr next(NodeTraits::get_next(prev_node));
      NodeTraits::set_previous(this_node, prev_node);
      NodeTraits::set_next(this_node, next);
      NodeTraits::set_previous(next, this_node);
      NodeTraits::set_next(prev_node, this_node);
   }

   //! <b>Requires</b>: this_node and other_node must be nodes inserted
   //!  in sequences or empty sequences.
   //! 
   //! <b>Effects</b>: Swaps the position of the nodes: this_node is inserted in
   //!   other_nodes position in seconds sequence and the other_node is inserted
   //!   in this_node's position in the second sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void swap_nodes(node_ptr this_node, node_ptr other_node)
   {
      if (other_node == this_node)
         return;
      bool empty1 = unique(this_node);
      bool empty2 = unique(other_node);

      node_ptr next_this(NodeTraits::get_next(this_node));
      node_ptr prev_this(NodeTraits::get_previous(this_node));
      node_ptr next_other(NodeTraits::get_next(other_node));
      node_ptr prev_other(NodeTraits::get_previous(other_node));

      //Do the swap
      NodeTraits::set_next(this_node, next_other);
      NodeTraits::set_next(other_node, next_this);

      NodeTraits::set_previous(this_node, prev_other);
      NodeTraits::set_previous(other_node, prev_this);

      if (empty2){
         init(this_node);
      }
      else{
         NodeTraits::set_next(prev_other, this_node);
         NodeTraits::set_previous(next_other, this_node);
      }
      if (empty1){
         init(other_node);
      }
      else{
         NodeTraits::set_next(prev_this, other_node);
         NodeTraits::set_previous(next_this, other_node);
      }
   }

   //! <b>Requires</b>: b and e must be nodes of the same sequence or an empty range.
   //!   and p must be a node of a different sequence.
   //! 
   //! <b>Effects</b>: Removes the nodes from [b, e) range from their sequence and inserts
   //!   them before p in p's sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void transfer(node_ptr p, node_ptr b, node_ptr e)
   {
      if (b != e) {
         node_ptr prev_p(NodeTraits::get_previous(p));
         node_ptr prev_e(NodeTraits::get_previous(e));
         node_ptr prev_b(NodeTraits::get_previous(b));
         NodeTraits::set_next(prev_e, p);
         NodeTraits::set_previous(p, prev_e);
         NodeTraits::set_next(prev_b, e);
         NodeTraits::set_previous(e, prev_b);
         NodeTraits::set_next(prev_p, b);
         NodeTraits::set_previous(b, prev_p);
      }
   }
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_LIST_ALGORITHMS_HPP
