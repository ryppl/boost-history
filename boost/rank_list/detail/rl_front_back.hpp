//  boost/rank_list/detail/rl_front_back.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_front_back.hpp
  ------------------------

  Methods for double ended queue interface support:

  front(): get reference to first element (O(1))
  front()_const_: idem but const_reference (O(1))
  push_front(): insert before first element (O(log N))
  pop_front(): remove first element (O(log N))
  back(): get reference to last element (O(1))
  back()_const_: idem but const_reference (O(1))
  push_back(): append after last element (O(log N))
  pop_back(): remove last element (O(log N))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_FRONT_BACK_HPP_
#define _BOOST_RANK_LIST_FRONT_BACK_HPP_

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

// front(): return (by reference) the first element
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reference
  rank_list<T,A,W,P>::front ()
{
  return *begin();
}

// front()_const_: return (by const reference) the first
// element
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::const_reference
  rank_list<T,A,W,P>::front ()                   const
{
  return *begin();
}

// push_front(): insert an element at the beginning
//
// Complexity: O(log(N))

template<class T,class A,class W,class P>
inline
  void
  rank_list<T,A,W,P>::push_front
  (typename rank_list<T,A,W,P>::const_reference t)
{
  insert (begin(), t);
}

// pop_front(): extract the first element (and simply
// delete it)
//
// Complexity: O(log(N))

template<class T,class A,class W,class P>
inline
  void
  rank_list<T,A,W,P>::pop_front ()
{
  erase (begin());
}

// back(): return (by reference) the last element
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reference
  rank_list<T,A,W,P>::back ()
{
  return *--end();
}

// back()_const_: return (by const reference) the last
// element
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::const_reference
  rank_list<T,A,W,P>::back ()                    const
{
  return *--end();
}

// push_back(): append an element at the end
//
// Complexity: O(log(N))

template<class T,class A,class W,class P>
inline
  void
  rank_list<T,A,W,P>::push_back
  (typename rank_list<T,A,W,P>::const_reference t)
{
  insert (end(), t);
}

// pop_back(): extract the last element (and simply
// delete it)
//
// Complexity: O(log(N))

template<class T,class A,class W,class P>
inline
  void rank_list<T,A,W,P>::pop_back ()
{
  erase (--end());
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

