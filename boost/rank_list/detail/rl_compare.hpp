//  boost/rank_list/detail/rl_compare.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_compare.hpp
  ---------------------

  Container comparison operators:

  == : size is == and all elements are ==  (O(min{M,N})*)
  != : not ==  (O(min{M,N})*)
  <  : 1st != is <, or all are == and size is <  (O(min{M,N})*)
  >  : 1st != is >, or all are == and size is >  (O(min{M,N})*)
  <= : not >  (O(min{M,N})*)
  >= : not <  (O(min{M,N})*)
  
  (*) M is the # of elements of one container and
      N is the # of elements of the other one
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_COMPARE_HPP_
#define _BOOST_RANK_LIST_COMPARE_HPP_

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

// operator==: compare the data contained here with the
// data of another rank_list, and return true iff every
// T element is equal to the corresponding element of the
// other container
//
// Complexity: O(N) (linear time)
// (where N is the number of T objects in the smaller
// rank_list)

template<class T,class A,class W,class P>
//not inline  MKR: should I make inline at least the 1st part?
  bool
  rank_list<T,A,W,P>::operator==
  (const typename rank_list<T,A,W,P>::my_class & a)   const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< EqualityComparableConcept<value_type> >();
#endif

  if (size()!=a.size()) return false; // If they have different
                                      // sizes, they can't be
  const_iterator i, j;                // equal

  for (i=begin(), j=a.begin(); i!=end(); ++i, ++j)
    if (!(*i==*j))
      return false;     // Compare and stop when the first
                        // difference is found
  return true;
}

// operator!=: see operator==
//
// Complexity: O(N) (linear time)
// (where N is the number of T objects in the smaller
// rank_list)

template<class T,class A,class W,class P>
inline
  bool
  rank_list<T,A,W,P>::operator!=
  (const typename rank_list<T,A,W,P>::my_class & a)   const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< EqualityComparableConcept<value_type> >();
#endif

  return !(*this==a);
}

// operator<: compare the data contained here with the data
// of another rank_list. Compare elements in sequence
// order until one element differs from the corresponding
// element of the other container. If it is lesser, return
// true; if it is greater, return false; if an end is
// reached the shortest sequence is lesser than the other
//
// Complexity: O(N) (linear time)
// (where N is the number of T objects in the smaller
// rank_list)

template<class T,class A,class W,class P>
//not inline  MKR: should I make inline at least the 1st part?
  bool
  rank_list<T,A,W,P>::operator<
  (const typename rank_list<T,A,W,P>::my_class & a)   const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< LessThanComparableConcept<value_type> >();
#endif

  if (!size()) return a.size()!=0;  // Both empty --> equal
                                    // This empty --> lesser
  const_iterator i, j;

  for (i=begin(), j=a.begin(); i!=end(); ++i, ++j)
    if (j==a.end() || *j<*i)
      return false;           // Greater than a
    else if (*i<*j)
      return true;            // Lesser than a

  return j!=a.end();          // Shorter --> lesser
}

// operator>: see operator<
//
// Complexity: O(N) (linear time)
// (where N is the number of T objects in the smaller
// rank_list)

template<class T,class A,class W,class P>
inline
  bool
  rank_list<T,A,W,P>::operator>
  (const typename rank_list<T,A,W,P>::my_class & a)   const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< LessThanComparableConcept<value_type> >();
#endif

  return a<*this;
}

// operator<=: see operator<
//
// Complexity: O(N) (linear time)
// (where N is the number of T objects in the smaller
// rank_list)

template<class T,class A,class W,class P>
inline
  bool
  rank_list<T,A,W,P>::operator<=
  (const typename rank_list<T,A,W,P>::my_class & a)   const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< LessThanComparableConcept<value_type> >();
#endif

  return !(a<*this);
}

// operator>=: see operator<
//
// Complexity: O(N) (linear time)
// (where N is the number of T objects in the smaller
// rank_list)

template<class T,class A,class W,class P>
inline
  bool
  rank_list<T,A,W,P>::operator>=
  (const typename rank_list<T,A,W,P>::my_class & a)   const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< LessThanComparableConcept<value_type> >();
#endif

  return !(*this<a);
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

