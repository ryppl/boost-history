// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

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
// Algorithms on sequences
//
// The functions in this file have not yet gone through formal
// review, and are subject to change. This is a work in progress.
// They have been checked into the detail directory because
// there are some graph algorithms that use these functions.

#include <algorithm>

namespace boost {

  template <class ForwardIterator, class T>
  void iota(ForwardIterator first, ForwardIterator last, T value)
  {
    for (; first != last; ++first, ++value)
      *first = value;
  }
 
  template <typename InputIterator, typename Predicate>
  bool any_if(InputIterator first, InputIterator last, Predicate p)
  {
    return std::find_if(first, last, p) != last;
  }
  template <typename Container, typename Predicate>
  bool any_if(const Container& c, Predicate p)
  {
    return any_if(begin(c), end(c), p);
  }

  template <typename InputIterator, typename T>
  bool contains(InputIterator first, InputIterator last, T value)
  {
    return std::find(first, last, value) != last;
  }

  template <typename InputIterator, typename Predicate>
  bool all(InputIterator first, InputIterator last, Predicate p)
  {
    for (; first != last; ++first)
      if (!p(*first))
        return false;
    return true;
  }

  template <typename InputIterator, typename Predicate>
  bool none(InputIterator first, InputIterator last, Predicate p)
  {
    return std::find_if(first, last, p) == last;
  }

  template <typename ForwardIterator>
  bool is_sorted(ForwardIterator first, ForwardIterator last)
  {
    if (first == last)
      return true;

    ForwardIterator next = first;
    for (++next; next != last; first = next, ++next) {
      if (*next < *first)
	return false;
    }

    return true;
  }

  template <typename ForwardIterator, typename StrictWeakOrdering>
  bool is_sorted(ForwardIterator first, ForwardIterator last,
		 StrictWeakOrdering comp)
  {
    if (first == last)
      return true;

    ForwardIterator next = first;
    for (++next; next != last; first = next, ++next) {
      if (comp(*next, *first))
	return false;
    }

    return true;
  }

} // namespace boost

#endif // BOOST_ALGORITHM_HPP
