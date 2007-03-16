//  boost/rank_list/detail/helper_fun_iter.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/helper_fun_iter.hpp
  --------------------------

  Private helper methods (of rank_list) for iterators.

  Iterators can't access the nodes' private members. They must
  use the helper methods implemented here. They also need
  rank_list as a friendship gateway between different iterators,
  because the 2-in-1 trick (used for declaring const and var
  iterators together) avoids declaring them friends of each other.
  The method data() is used in rank_list too (index operators)

  next(): get the next node of a given node (O(1))
  prev(): get the previous node of a given node (O(1))
  data(): get (by ref) the data of a node (with data!) (O(1))
  iterator_pointer(): get the node refered by an it. (O(1))
  make_const_iterator(): get const it. referring a node (O(1))
  make_const_rev_iter(): get const reverse it... (O(1))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_HELPER_FUN_ITER_HPP_
#define _BOOST_RANK_LIST_HELPER_FUN_ITER_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {

//////////////////////////////////////////////////////////////////

// next(): Get the next node of a given node
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::node_t *
  rank_list<T,A,W,P>::next
  (typename rank_list<T,A,W,P>::node_t * p)
{
  RL_ASSERT (p);       // NULL pointer dereference
  return p->m_next;
}

// prev(): Get the previous node of a given node
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::node_t *
  rank_list<T,A,W,P>::prev
  (typename rank_list<T,A,W,P>::node_t * p)
{
  RL_ASSERT (p);       // NULL pointer dereference
  return p->m_prev;
}

// data(): Get a reference to the payload data (value_type)
// of a given node
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::reference
  rank_list<T,A,W,P>::data
  (typename rank_list<T,A,W,P>::node_t * p)
{
  RL_ASSERT (p);            // NULL pointer dereference

  RL_ASSERT_EXC (p->m_parent,
                 invalid_op_with_end());   // end() has no data

  return (static_cast<payload_node_t*>(p))->m_data;
}

// iterator_pointer(): return the node pointer of an
// iterator. This is required by iterator classes
// for reading the pointer of other iterator classes
//
// Complexity: O(1)

template<class T,class A,class W,class P>
template<class IT>
inline //static
  typename rank_list<T,A,W,P>::node_t *
  rank_list<T,A,W,P>::iterator_pointer
  (const IT & it)
{
#ifdef BOOST_CLASS_REQUIRE
#ifdef RL_USE_RANDOM_ACCESS_TAG
  function_requires<
      RandomAccessIteratorConcept<IT> >();
#else
  function_requires<
      BidirectionalIteratorConcept<IT> >();
#endif
#endif

  return it.ptr;
}

// make_const_iterator(): call the private constructor
// of the class const_iterator. This is required by class
// iterator for converting iterators to const_iterators
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::const_iterator
  rank_list<T,A,W,P>::make_const_iterator
  (typename rank_list<T,A,W,P>::node_t * p)
{
  return const_iterator(p);
}

// make_const_reverse_iterator(): call the private
// constructor of the class const_iterator. This is
// required by class reverse_iterator for converting
// reverse iterators to const_reverse_iterators
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline //static
  typename rank_list<T,A,W,P>::const_reverse_iterator
  rank_list<T,A,W,P>::make_const_rev_iter
  (typename rank_list<T,A,W,P>::node_t * p)
{
  return const_reverse_iterator(p);
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

