//  boost/rank_list/detail/rl_size.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_size.hpp
  ------------------

  Methods related to the size of the tree (get/change size):

  size(): retrieve current size (O(1))
  empty(): true if empty; false otherwise (O(1))
  max_size(): estimated maximum size in theory (O(1))
  resize(n): change size (O(min{N, n log N}))
  resize(n,t): idem, but add copies of t  "

  Private helper methods:

  resize(n,DP): change size (rebuild) O(max{old_size,new_size})
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_SIZE_HPP_
#define _BOOST_RANK_LIST_SIZE_HPP_

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

// size(): number of elements contained
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::size_type
  rank_list<T,A,W,P>::size () const
{
  return node_t::m_count-1;
}

// empty(): inform of whether the container is empty or not
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  bool
  rank_list<T,A,W,P>::empty () const
{
  return size()==0;
}

// max_size(): estimated maximum size (in theory) supposing
// that a whole address space is available (which is
// obviously impossible) and taking into account that
// end()-begin() should fit in difference_type
//
// Complexity: O(1)

template<class T,class A,class W,class P>
//not inline
  typename rank_list<T,A,W,P>::size_type
  rank_list<T,A,W,P>::max_size ()
{
                             // If pointers are smaller or eq.
                             // to size_type, the limit is
                             // imposed by the address space

  if (sizeof(void*)<=sizeof(size_type))
    return ( ( size_type(1) <<
               ((sizeof(void*)<<3)-1) ) /
             sizeof(payload_node_t)       ) << 1;

  size_type mxp, mxu, r, mn;    // Otherwise, it depends on
                                         // the sizes ratio
  mxp = ( ( size_type(1) <<
            ((sizeof(size_type)<<3)-1) ) /
          sizeof(payload_node_t)           ) << 1;

  mn = sizeof(difference_type) < sizeof(size_type) ?
       sizeof(difference_type) : sizeof(size_type);

  mxu = size_type(1) << ((mn<<3)-2);

  r =  ( sizeof(void*) -            // Using r we avoid
         sizeof(size_type) ) << 3;  // overflow on mxp

  return (mxu>>r) < mxp ? // If the index type size is more
         mxu :            // restrictive, choose it. Otherwise
         (mxp<<r);        // choose the address space limit
}

// resize(): change the size of the rank_list, deleting
// elements from the end, or appending copies of t
// (depending on the specified new size n)
//
// Complexity: (O(min{N, n log N}))

template<class T,class A,class W,class P>
//not inline
  void
  rank_list<T,A,W,P>::resize
  (typename rank_list<T,A,W,P>::size_type n,
   typename rank_list<T,A,W,P>::const_reference t)
{
  size_type sz=size();                  // If there's a big
                                        // difference with
  if (n<=0)                             // respect to the
    clear ();                           // current size, resize
  else if ((n>sz &&                     // building (linking)
            worth_rebuild(n-sz,sz)) ||  // the whole tree again
           (n<sz &&
            worth_rebuild(sz-n,sz,true)))
  {
    copy_data_provider<const_pointer> dp(&t);
    resize (n, dp);
  }
  else if (n>sz)                    // If the difference
    insert (end(), n-size(), t);    // is small,
  else if (n<sz)                    // append or remove
    erase (begin()+n, end());       // elements one by one
}

// resize(): change the size of the rank_list, deleting
// elements from the end, or appending default constructed
// T objects (depending on the specified new size n)
//
// Complexity: (O(min{N, n log N}))

template<class T,class A,class W,class P>
//not inline
  void
  rank_list<T,A,W,P>::resize
  (typename rank_list<T,A,W,P>::size_type n)
{
  null_data_provider<const_pointer> dp;
  node_t * first, * last, * p;
  size_type sz=size();                  // If there's a big
                                        // difference with
  if (n<=0)                             // respect to the
    clear ();                           // current size, resize
  else if ((n>sz &&                     // building (linking)
            worth_rebuild(n-sz,sz)) ||  // the whole tree again
           (n<sz &&
            worth_rebuild(sz-n,sz,true)))
    resize (n, dp);
  else if (n<sz)                        // Otherwise append or
    erase (begin()+n, end());           // remove elements one
  else if (n>sz)                        // by one
  {
    construct_nodes_list (first, last, n-sz, dp, false);

    while (first)
    {
      p = first;
      first = first->m_next;
      insert_before (p, dummy());
    }
  }
}


// ------------------- PRIVATE HELPER METHODS --------------------

// resize(n,DP): change the size of the rank_list, deleting
// elements from the end, or appending new elements
// (depending on the specified new size n).
// Do it rebuilding the whole tree in all cases
//
// Complexity: O(max{old_size,new_size})

template<class T,class A,class W,class P>
template<class DP>
  void
  rank_list<T,A,W,P>::resize
  (typename rank_list<T,A,W,P>::size_type n,
   DP & dp)
{
  node_t * first, * last;
  node_t * p, * next;
                                  // Build a new tree adding
  if (n>size())                   // and/or recycling nodes
  {
    construct_nodes_list (first, last, n-size(), dp);
    node_t::m_prev->m_next = first;
    build_known_size_tree (n, node_t::m_next);
  }
  else
  {
    node_t::m_prev->m_next = NULL;
    next = build_known_size_tree (n, node_t::m_next);

    while (next)                  // Destruct remaining
    {                             // old elements
      p = next;
      next = next->m_next;
      delete_node (p);
    }
  }
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

