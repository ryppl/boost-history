//  boost/rank_list/detail/helper_fun_node.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/helper_fun_node.hpp
  --------------------------

  Private helper methods for the relationship container-nodes

  dummy(): get the sentinel/end node of this container (O(1))
  dummy_ownwer(): get the container of a dummy node (O(1))
  owner(): get the container of a node (O(log N))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_HELPER_FUN_NODE_HPP_
#define _BOOST_RANK_LIST_HELPER_FUN_NODE_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {

//////////////////////////////////////////////////////////////////

// Most data stored inside the rank_list object come from the base
// class: a node. This node is the root of the whole tree and the
// end element at the same time. It is the only node whose parent
// pointer is NULL. Its right child pointer will allways be NULL.
// Last, but not least, it has no T data. The method dummy()
// returns a pointer to this sentinel/end node
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::node_t *
  rank_list<T,A,W,P>::dummy () const
{
  return static_cast<node_t*> (
         const_cast<my_class*> (this) );
}

// The static method dummy_owner() calculates the
// address of a rank_list from its dummy's address
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::my_class *
  rank_list<T,A,W,P>::dummy_owner
  (const typename rank_list<T,A,W,P>::node_t * pdummy)
{
  RL_ASSERT (!pdummy->m_parent);

  return static_cast<my_class*> (
         const_cast<node_t*> (pdummy) );
}

// Finally, the static method owner() returns the address
// of the rank_list to which a node belongs. This takes
// some time
//
// Complexity: O(log N)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::my_class *
  rank_list<T,A,W,P>::owner
  (const typename rank_list<T,A,W,P>::node_t * node)
{
  while (node->m_parent)
    node = node->m_parent;

  return dummy_owner (node);
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

