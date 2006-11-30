//  boost/rank_list/detail/rl_alloc.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_alloc.hpp
  -------------------

  Private helper methods for nodes allocation/deallocation

  new_node(): Allocate and construct a new node (O(1))
  delete_node(): Destruct and deallocate a node (O(1))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_ALLOC_HPP_
#define _BOOST_RANK_LIST_ALLOC_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    using namespace detail;
    
//////////////////////////////////////////////////////////////////

// ------------------- PRIVATE HELPER METHODS --------------------

// new_node(): Allocate and construct a new node. If the parameter
// t is null, use T's default constructor. If it is not NULL (it
// points to a valid T object), use T's copy constructor.
// Note: T's constructors are not called directly, but through the
// node constructors
//
// Complexity: O(1) (regarded that T's constructor is O(1) ;)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::node_t *
  rank_list<T,A,W,P>::new_node
  (typename rank_list<T,A,W,P>::const_pointer t)
{
  payload_node_t * p;

  p = allocator.allocate (1); // Just one

  if (p==NULL)                       // If the allocator didn't
    throw allocator_returned_null(); // throw an exception, but
                                     // it returned NULL, throw
  if (t)
    new (p) payload_node_t(*t);      // Call the constructor
  else                               // through the placement
    new (p) payload_node_t;          // new operator

  return static_cast<node_t*>(p);    // Return allocated node
}

// delete_node(): Destruct and deallocate an existing node
//
// Complexity: O(1) (regarded that T's destructor is O(1) ;)

template<class T,class A,class W,class P>
inline void
  rank_list<T,A,W,P>::delete_node
  (typename rank_list<T,A,W,P>::node_t * p)
{
  RL_ASSERT (p);
  payload_node_t * q = static_cast<payload_node_t*>(p);
  q->~payload_node_t ();
  allocator.deallocate (q, 1);
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

