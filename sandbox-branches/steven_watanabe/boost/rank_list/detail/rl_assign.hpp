//  boost/rank_list/detail/rl_assign.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_assign.hpp
  --------------------

  Container assignment and swap operations:

  Container assignment (O(M+N): M to delete + N to copy)
  Container swap (O(1))

  Private helper method:

  acquire_tree(): steal the tree of another rank_list (O(1))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_ASSIGN_HPP_
#define _BOOST_RANK_LIST_ASSIGN_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    using namespace detail;

//////////////////////////////////////////////////////////////////

// ---------------------- PUBLIC INTERFACE -----------------------

// Assignment operator: replace the current contents of the
// rank_list with a copy of the contents of a. The T
// objects will be copied one by one, in sequence order,
// using the T copy constructor. The resulting tree will be
// perfectly balanced.
//
// Complexity: O(M+N) (linear time)
// (where M is the number of T objects to delete, and N is
// the number of T objects to copy)

template<class T,class A,class W,class P>
inline
  const typename rank_list<T,A,W,P>::my_class &
  rank_list<T,A,W,P>::operator=
  (const typename rank_list<T,A,W,P>::my_class & a)
{
  node_t * first, * last;
  iter_data_provider<const_pointer,
                     const_iterator> dp(a.begin());

  if (&a == this) return *this;
  construct_nodes_list (first, last, a.size(), dp);
  clear ();
  build_known_size_tree (a.size(), first);
  return *this;
}

// swap(): interchange the contents of two rank_list
// containers. This operation only requires changing some
// pointers. T objects are not touched
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  void
  rank_list<T,A,W,P>::swap
  (typename rank_list<T,A,W,P>::my_class & a)
{
  node_t tmp;

  if (&a == this) return;  // Self-swap is nonsense

  tmp = *dummy ();           // tmp <-- *this
  acquire_tree (*a.dummy()); // *this <-- a
  a.acquire_tree (tmp);      // a <-- tmp
}


// ------------------- PRIVATE HELPER METHODS --------------------

// acquire_tree(): steal the whole tree of another rank_list,
// linking it to *this. Ignore any previous contents of *this.
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  void
  rank_list<T,A,W,P>::acquire_tree
  (const typename rank_list<T,A,W,P>::node_t & nf)
{
  if (!nf.m_children[L])   // If the tree to acquire is empty,
    init ();            // just initialize
  else
  {
    *dummy () = nf;               // Link dummy to the tree

    node_t::m_children[L]->m_parent =    // Link the tree to dummy
    node_t::m_prev->m_next =
    node_t::m_next->m_prev = dummy ();

    node_t::m_total_width =                         // Copy total
           node_t::m_children[L]->m_total_width;    // width into
  }                                                 // dummy node
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

