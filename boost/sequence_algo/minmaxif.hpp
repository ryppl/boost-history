// minmaxif.hpp -- min_ and max_element_if functions. Originally in minmax.hpp
// (C) Copyright Hervé Brönnimann, Polytechnic University, 2002--2004
//
// $log$

#ifndef BOOST_MINMAXIF_HPP
#define BOOST_MINMAXIF_HPP

/* PROPOSED STANDARD EXTENSIONS:
 *
 * min_element_if(first, last, predicate)
 * Effect: returns the smallest element of the subsequence of [first,last)
 *         satisfying the predicate (or last if none)
 *
 * min_element_if(first, last, comp, predicate)
 * Effect: returns the smallest element, for the order comp, of the subsequence
 *         of [first,last) satisfying the predicate (or last if none)
 *
 * max_element_if(first, last, predicate)
 * Effect: returns the largest element of the subsequence of [first,last)
 *         satisfying the predicate (or last if none)
 *
 * max_element_if(first, last, comp, predicate)
 * Effect: returns the largest element, for the order comp, of the subsequence
 *         of [first,last) satisfying the predicate (or last if none)
 */

  template <class ForwardIter, class UnaryPredicate>
  ForwardIter
  min_element_if(ForwardIter first, ForwardIter last, UnaryPredicate cond)
  {
    while (first != last && !cond(*first))
      ++first;
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
      if (cond(*first) && *first < *min_result)
        min_result = first;
    return min_result;
  }	// min_element_if

  template <class ForwardIter, class BinaryPredicate, class UnaryPredicate>
  ForwardIter
  min_element_if(ForwardIter first, ForwardIter last,
                 BinaryPredicate comp, UnaryPredicate cond)
  {
    while (first != last && !cond(*first))
      ++first;
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
      if (cond(*first) && comp(*first, *min_result))
        min_result = first;
    return min_result;
  }	// min_element_if

  template <class ForwardIter, class UnaryPredicate>
  ForwardIter
  max_element_if(ForwardIter first, ForwardIter last, UnaryPredicate cond)
  {
    while (first != last && !cond(*first))
      ++first;
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
      if (cond(*first) && *max_result < *first)
        max_result = first;
    return max_result;
  }	// max_element_if

  template <class ForwardIter, class BinaryPredicate, class UnaryPredicate>
  ForwardIter
  max_element_if(ForwardIter first, ForwardIter last,
                 BinaryPredicate comp, UnaryPredicate cond)
  {
    while (first != last && !cond(*first))
      ++first;
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
      if (cond(*first) && comp(*max_result, *first))
        max_result = first;
    return max_result;
  }	// max_element_if

#endif  // BOOST_MINMAXIF_HPP
