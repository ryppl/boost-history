// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// Revision History:

// 16 Mar 2003  pair<min,max> min_max_element should be implemented.
// 27 Jun 2002	Herve Bronnimann
//    Added concept checking
// 27 Jun 2002  Herve Bronnimann
//    Shorter implementation of is_sorted

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef BOOST_ALGORITHM_HPP
# define BOOST_ALGORITHM_HPP

# include <boost/detail/iterator.hpp>
# include <boost/concept_check.hpp>

// Algorithms on sequences
//
// The functions in this file have not yet gone through formal
// review, and are subject to change. This is a work in progress.
// They have been checked into the detail directory because
// there are some graph algorithms that use these functions.


namespace boost {

// The following should probably eventually be added to the concept checking
// library, as well as PostIncrementOpConcept, and idem with Decrement
// -- Herve

#define BOOST_DEFINE_UNARY_PRE_INCREMENT_OP_CONSTRAINT(OP, NAME) \
  template <class TT> \
  struct NAME { \
    void constraints() { \
      OP a; \
    } \
    TT a; \
  }
  BOOST_DEFINE_UNARY_PRE_INCREMENT_OP_CONSTRAINT(++, PreIncrementOpConcept);
#undef BOOST_DEFINE_UNARY_PRE_INCREMENT_OP_CONSTRAINT

  template <class ForwardIterator, class T>
  void iota(ForwardIterator first, ForwardIterator last, T value)
  {
    function_requires< ForwardIteratorConcept<ForwardIterator> >();
    function_requires< PreIncrementOpConcept<T> >();
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
    function_requires< ConvertibleConcept<T,
      typename std::iterator_traits<ForwardIterator>::value_type > >();
#endif
    for (; first != last; ++first, ++value)
      *first = value;
  }
 
  template <typename InputIterator, typename T>
  bool contains(InputIterator first, InputIterator last, T value)
  {
    function_requires< InputIteratorConcept<InputIterator> >();
    return std::find(first, last, value) != last;
  }

  template <typename InputIterator, typename Predicate>
  bool all(InputIterator first, InputIterator last, Predicate p)
  {
    function_requires< InputIteratorConcept<InputIterator> >();
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
    function_requires< UnaryPredicateConcept<Predicate,
      typename std::iterator_traits<InputIterator>::value_type > >();
#endif
    for (; first != last; ++first)
      if (!p(*first))
        return false;
    return true;
  }

  template <typename InputIterator, typename Predicate>
  bool none(InputIterator first, InputIterator last, Predicate p)
  {
    function_requires< InputIteratorConcept<InputIterator> >();
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
    function_requires< UnaryPredicateConcept<Predicate,
      typename std::iterator_traits<InputIterator>::value_type > >();
#endif
    return std::find_if(first, last, p) == last;
  }

  template <typename InputIterator, typename Predicate>
  bool any_if(InputIterator first, InputIterator last, Predicate p)
  {
    function_requires< InputIteratorConcept<InputIterator> >();
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
    function_requires< UnaryPredicateConcept<Predicate,
      typename std::iterator_traits<InputIterator>::value_type > >();
#endif
    return std::find_if(first, last, p) != last;
  }

  template<class ForwardIterator>
  inline bool
  is_sorted(ForwardIterator first, ForwardIterator last)
  {
    function_requires< ForwardIteratorConcept<ForwardIterator> >();
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
    function_requires< LessThanComparableConcept<
      typename std::iterator_traits<ForwardIterator>::value_type > >();
#endif
    if (first != last)
      for (ForwardIterator old = first; ++first != last; old = first)
        if (*first < *old)
          return false;
    return true;
  }

  template<class ForwardIterator, class StrictWeakOrdering>
  inline bool
  is_sorted(ForwardIterator first, ForwardIterator last,
            StrictWeakOrdering comp)
  {
    function_requires< ForwardIteratorConcept<ForwardIterator> >();
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
    function_requires< BinaryPredicateConcept<StrictWeakOrdering,
      typename std::iterator_traits<ForwardIterator>::value_type,
      typename std::iterator_traits<ForwardIterator>::value_type > >();
#endif
    if (first != last)
      for (ForwardIterator old = first; ++first != last; old = first)
        if (comp(*first, *old))
          return false;
    return true;
  }

} // namespace boost

#endif // BOOST_ALGORITHM_HPP
