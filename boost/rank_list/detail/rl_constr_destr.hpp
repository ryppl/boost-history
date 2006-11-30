//  boost/rank_list/detail/rl_constr_destr.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_constr_destr.hpp
  --------------------------

  Constructors and destructor:

  Default con.: empty rank_list (O(1))
  Copy con.: copy of other rank_list (O(N))
  Vector con.: rank_list with n elements like t (O(N))
  Vector def. con.: " with n default-constructed elem. (O(N))
  Sequence con.: " with copies of [from,to) (O(N))
  Sequence con.: " with copies of [from,from+n) (O(N))
  Destructor (O(N))

  Private helper method:

  init(): initialize a rank_list with "empty" state (O(1))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_CONSTR_DESTR_HPP_
#define _BOOST_RANK_LIST_CONSTR_DESTR_HPP_

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

// Default constructor: create an empty rank_list
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::rank_list ()
{
  init ();
}

// Copy constructor: create a rank_list copying the
// contents of a. The T objects will be copied one by
// one, in sequence order, using the T copy constructor.
// The resulting tree will be perfectly balanced.
//
// Complexity: O(N)

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::rank_list
  (const typename rank_list<T,A,W,P>::my_class & a)
{
  node_t * first, * last;
  iter_data_provider<const_pointer,
                     const_iterator> dp(a.begin());

  construct_nodes_list (first, last, a.size(), dp);
  build_known_size_tree (a.size(), first);
}

// Vector constructor: create a rank_list with n
// elements, all them copies of t. The T objects will be
// created one by one using the T copy constructor. Three
// versions of this constructor are provided to avoid
// conflicts with some template methods provided bellow
//
// Complexity: O(N)

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::rank_list
  (typename rank_list<T,A,W,P>::size_type n,
   typename rank_list<T,A,W,P>::const_reference t)
{
  node_t * first, * last;
  copy_data_provider<const_pointer> dp(&t);

  construct_nodes_list (first, last, n, dp);
  build_known_size_tree (n, first);
}

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::rank_list
  (int n,
   typename rank_list<T,A,W,P>::const_reference t)
{
  node_t * first, * last;
  copy_data_provider<const_pointer> dp(&t);

  if (n<0) n = 0;
  construct_nodes_list (first, last, n, dp);
  build_known_size_tree (n, first);
}

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::rank_list
  (long n,
   typename rank_list<T,A,W,P>::const_reference t)
{
  node_t * first, * last;
  copy_data_provider<const_pointer> dp(&t);

  if (n<0) n = 0;
  construct_nodes_list (first, last, n, dp);
  build_known_size_tree (n, first);
}

// Vector constructor: create a rank_list with n
// elements, all them created with T's default
// constructor
//
// Complexity: O(N)

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::rank_list
  (typename rank_list<T,A,W,P>::size_type n)
{
  node_t * first, * last;
  null_data_provider<const_pointer> dp;

  construct_nodes_list (first, last, n, dp);
  build_known_size_tree (n, first);
}

// Sequence [freom,to) constructor: create a rank_list with an
// undetremined number of elements copied from the sequence
// [from,to). The parameters can be iterators of any type
// (including iterators of other containers).
//
// Complexity: O(N)

template<class T,class A,class W,class P>
template <class IT>
inline
  rank_list<T,A,W,P>::rank_list (IT from, IT to)
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< InputIteratorConcept<IT> >();
#endif

  node_t * first, * last;
  range_data_provider<const_pointer,IT> dp(from,to);
  size_type n;

  n = construct_nodes_list (first, last, 0, dp, false, true);
  build_known_size_tree (n, first);
}

// Sequence [from,from+n) constructor: create a rank_list with
// a known number of elements copied from the sequence that
// starts in [from].
//
// Complexity: O(N)

template<class T,class A,class W,class P>
template <class IT>
inline
  rank_list<T,A,W,P>::rank_list
  (IT from,
   typename rank_list<T,A,W,P>::size_type n)
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< InputIteratorConcept<IT> >();
#endif

  node_t * first, * last;
  iter_data_provider<const_pointer,IT> dp(from);

  construct_nodes_list (first, last, n, dp);
  build_known_size_tree (n, first);
}

// Destructor: deallocate contents
//
// Complexity: O(N)

template<class T,class A,class W,class P>
inline
  rank_list<T,A,W,P>::~rank_list ()
{
  clear ();  // (See impl. of clear() in erase.hpp)
}


// ------------------- PRIVATE HELPER METHODS --------------------

// init(): initialize a rank_list with default values,
// corresponding to an "empty" state (O(1), regarded that W's
// constructor is O(1) ;)

template<class T,class A,class W,class P>
inline void
  rank_list<T,A,W,P>::init ()
{
  node_t::m_parent =
  node_t::m_children[0] =
  node_t::m_children[1] = NULL;               // Lonely node

  node_t::m_next = node_t::m_prev = dummy (); // List: loop
  node_t::m_count = node_t::m_height = 1;     // Nodes: one (dummy)

  node_t::m_node_width =
  node_t::m_total_width = W(0);               // Zero width

  m_sums_out_of_date = false;                 // Sums up to date
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

