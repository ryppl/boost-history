//  boost/rank_list/detail/node.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/node.hpp
  ---------------

  The class rank_list_node_tree_fields, defined here, contains all
  the links required by tree nodes. It does _not_ contain the
  payload value_type (see detail/node_with_data.hpp).

  Two classes inherit from rank_list_node_tree_fields:

    rank_list_node   (see detail/node_with_data.hpp)
    rank_list        (see rank_list.hpp)
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_NODE_HPP_
#define _BOOST_RANK_LIST_NODE_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    namespace detail
    {

//////////////////////////////////////////////////////////////////
/*
  The type enum_left_right declares L and R.
  Every tree node stores the pointers to its children in an rlist
  of two pointers. L and R (constant values) are often used as
  index in the rlist, but in some cases the index is a variable.
  We can take advantage of symmetry: if children[x] is one side,
  children[1-x] is the other one.
*/

typedef enum { L=0, R=1 } enum_left_right;

//////////////////////////////////////////////////////////////////

template<class T, class A,        // Data of a tree node (payload
         class W, class P>        // not included)
class rank_list_node_tree_fields
{                                 // Note that the dummy has no T

  friend class boost::rank_lists::rank_list<T,A,W,P>;
  friend class rollback_list<T,A,W,P>;

  typedef rank_list_node_tree_fields<T,A,W,P>    node_t;

  protected:

    // Tree links (parent of root and children of leafs are NULL)

    node_t * m_parent;      // parent node
    node_t * m_children[2]; // [0]:left [1]:right

    // Circular doubly linked list (equiv. to in-order travel)

    node_t * m_next;        // (last_node.next==dummy)
    node_t * m_prev;        // (first_node.prev==dummy)

    // Data for balancing, indexing, and stable-sort

    std::size_t m_height;   // levels in subtree, including self
    std::size_t m_count;    // nodes in subtree, including self
    P m_oldpos;             // position (used only in stable_sort)

                      // Alternative sequence view:
    W m_node_width;   // Width of this node
    W m_total_width;  // Width of this subtree

    // Constructor and initializer, both O(1)

    rank_list_node_tree_fields ();     // Default constructor
    void init ();                      // Write default values

    // Helper functions, all O(1) (no loop, no recursion)

    std::size_t left_count () const;   // Nodes in left subtree
    std::size_t right_count () const;  // Nodes in right subtree

    std::size_t left_height () const;  // Height of left subtree
    std::size_t right_height () const; // Height of right subtree

    W left_width () const;             // Width of left subtree
    W right_width () const;            // Width of right subtree
};

//////////////////////////////////////////////////////////////////

// Initializer, or "reset" method: write default values

template<class T,class A,class W,class P>
inline void
  rank_list_node_tree_fields<T,A,W,P>::
  init ()                                // Write default values
{
  m_parent =
  m_children[L] = m_children[R] = NULL; // No relatives

  m_next = m_prev = this;     // Loop list
  m_height = m_count = 1;     // Single element, single level

  m_node_width = m_total_width = W(1);  // Default
}                                       // width

// Constructor: just call init()

template<class T,class A,class W,class P>
inline
  rank_list_node_tree_fields<T,A,W,P>::
  rank_list_node_tree_fields ()
{ init (); }

// Helper functions: return count/height/width of left/right
// subtree. This doesn't require loops or recursion. If the
// left/right subtree is empty, return 0; otherwise, return
// the count/height/width of its root. Time required is O(1)

template<class T,class A,class W,class P>
inline std::size_t
  rank_list_node_tree_fields<T,A,W,P>::
  left_count ()                         const
{
  return m_children[L] ?
         m_children[L]->m_count : 0;
}

template<class T,class A,class W,class P>
inline std::size_t
  rank_list_node_tree_fields<T,A,W,P>::
  right_count ()                        const
{
  return m_children[R] ?
         m_children[R]->m_count : 0;
}

template<class T,class A,class W,class P>
inline std::size_t
  rank_list_node_tree_fields<T,A,W,P>::
  left_height ()                        const
{
  return m_children[L] ?
         m_children[L]->m_height : 0;
}

template<class T,class A,class W,class P>
inline std::size_t
  rank_list_node_tree_fields<T,A,W,P>::
  right_height ()                       const
{
  return m_children[R] ?
         m_children[R]->m_height : 0;
}

template<class T,class A,class W,class P>
inline W
  rank_list_node_tree_fields<T,A,W,P>::
  left_width ()                         const
{
  return m_children[L] ?
         m_children[L]->m_total_width : W(0);
}

template<class T,class A,class W,class P>
inline W
  rank_list_node_tree_fields<T,A,W,P>::
  right_width ()                        const
{
  return m_children[R] ?
         m_children[R]->m_total_width : W(0);
}

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

