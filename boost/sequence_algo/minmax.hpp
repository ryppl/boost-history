/* (C) Copyright Hervé Brönnimann, Polytechnic University, 2002.
 *
 * Permission to copy, use, modify, sell and distribute this software
 * and its documentation is granted provided this copyright notice
 * appears in all copies. This software and its documentation is
 * provided "as is" without express or implied warranty, and with no
 * claim as to its suitability for any purpose.
 */

#ifndef BOOST_MINMAX_H
#define BOOST_MINMAX_H

#include <utility>

/* PROPOSED STANDARD EXTENSIONS:
 *
 * minmax(a, b)
 * Effect: (b<a) ? std::make_pair(b,a) : std::make_pair(a,b);
 *
 * minmax(a, b, comp)
 * Effect: comp(b,a) ? std::make_pair(b,a) : std::make_pair(a,b);
 *
 * minmax_element(first, last)
 * Effect: std::make_pair( std::min_element(first, last),
 *                         std::max_element(first, last) );
 *
 * minmax_element(first, last, comp)
 * Effect: std::make_pair( std::min_element(first, last, comp),
 *                         std::max_element(first, last, comp) );
 */

namespace boost {

  template <class T>
  std::pair<const T&,const T&>
  minmax(const T& a, const T& b) {
    if (b<a) return std::make_pair(b,a);
    else     return std::make_pair(a,b);
  }

  template <class T, class BinaryPredicate>
  std::pair<const T&,const T&>
  minmax(const T& a, const T& b, BinaryPredicate comp) {
    if (comp(b,a)) return std::make_pair(b,a);
    else           return std::make_pair(a,b);
  }

  template <class ForwardIter>
  std::pair<ForwardIter,ForwardIter>
  minmax_element(ForwardIter first, ForwardIter last)
  {
    if (first == last) return std::make_pair(last,last);
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // if only one element
    ForwardIter second = first; ++second;
    if (second == last) return std::make_pair(min_result, max_result);
    // treat first pair separately (only one comparison for first two elements)
    ForwardIter potential_min_result = last;
    if (*first < *second)
      max_result = second;
    else
      min_result = second, potential_min_result = first;
    // then each element by pairs, with at most 3 comparisons per pair
    first = ++second; if (first != last) ++second;
    while (second != last) {
      if (*first < *second) {
        if (*first < *min_result)
	  min_result = first, potential_min_result = last;
	if (*max_result < *second)
	  max_result = second;
      } else {
	// could be that *first == *second
        if (*second < *min_result)
	  // Glisse: min_result = (*first > *second) ? second : first;
	  min_result = second, potential_min_result = first;
	if (*max_result < *first)
	  max_result = first;
      }
      first = ++second; if (first != last) ++second;
    }
    // if odd number of elements, treat last element, with at most 2 comparisons
    if (first != last) {
      if (*first < *min_result)
	min_result = first, potential_min_result = last;
      else if (*max_result < *first)
	max_result = first;
    }
    // resolve min_result being incorrect, with one extra comparison
    // (in which case potential_min_result is necessarily the correct result)
    if (potential_min_result != last && !(*min_result < *potential_min_result))
      min_result = potential_min_result;
    return std::make_pair(min_result,max_result);
  }

  template <class ForwardIter, class BinaryPredicate>
  std::pair<ForwardIter,ForwardIter>
  minmax_element(ForwardIter first, ForwardIter last,
                 BinaryPredicate comp)
  {
    if (first == last) return std::make_pair(last,last);
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // if only one element
    ForwardIter second = first; ++second;
    if (second == last) return std::make_pair(min_result, max_result);
    // treat first pair separately (only one comparison for first two elements)
    ForwardIter potential_min_result = last;
    if (comp(*first, *second))
      max_result = second;
    else
      min_result = second, potential_min_result = first;
    // then each element by pairs, with at most 3 comparisons per pair
    first = ++second; if (first != last) ++second;
    while (second != last) {
      if (comp(*first, *second)) {
        if (comp(*first, *min_result))
	  min_result = first, potential_min_result = last;
        if (comp(*max_result, *second))
	  max_result = second;
      } else {
        if (comp(*second, *min_result))
	  min_result = second, potential_min_result = first;
        if (comp(*max_result, *first))
	  max_result = first;
      }
      first = ++second; if (first != last) ++second;
    }
    // if odd number of elements, treat last element
    if (first != last) { // odd number of elements
      if (comp(*first, *min_result))
	min_result = first, potential_min_result = last;
      else if (comp(*max_result, *first))
	max_result = first;
    }
    // resolve min_result being incorrect with one extra comparison
    // (in which case potential_min_result is necessarily the correct result)
    if (potential_min_result != last
        && !comp(*min_result, *potential_min_result))
      min_result = potential_min_result;
    return std::make_pair(min_result,max_result);
  }

}
 
/* PROPOSED BOOST EXTENSIONS
 * In the description below, [rfirst,rlast) denotes the reversed range
 * of [first,last). Even though the iterator type of first and last may
 * be only a Forward Iterator, it is possible to explain the semantics
 * by assuming that it is a Bidirectional Iterator. In the sequel,
 * reverse(ForwardIterator&) returns the reverse_iterator adaptor.
 * This is not how the functions would be implemented!
 *
 * first_min_element(first, last)
 * Effect: std::min_element(first, last);
 *                    
 * first_min_element(first, last, comp)
 * Effect: std::min_element(first, last, comp);
 *
 * last_min_element(first, last)
 * Effect: reverse( std::min_element(reverse(last), reverse(first)) );
 *                    
 * last_min_element(first, last, comp)
 * Effect: reverse( std::min_element(reverse(last), reverse(first), comp) );
 *
 * first_max_element(first, last)
 * Effect: std::max_element(first, last);
 *                    
 * first_max_element(first, last, comp)
 * Effect: max_element(first, last);
 *
 * last_max_element(first, last)
 * Effect: reverse( std::max_element(reverse(last), reverse(first)) );
 *                    
 * last_max_element(first, last, comp)
 * Effect: reverse( std::max_element(reverse(last), reverse(first), comp) );
 *
 * first_min_first_max_element(first, last)
 * Effect: std::make_pair( first_min_element(first, last),
 *                         first_max_element(first, last) );
 *                    
 * first_min_first_max_element(first, last, comp)
 * Effect: std::make_pair( first_min_element(first, last, comp),
 *                         first_max_element(first, last, comp) );
 *
 * first_min_last_max_element(first, last)
 * Effect: std::make_pair( first_min_element(first, last),
 *                         last_max_element(first, last) );
 *                    
 * first_min_last_max_element(first, last, comp)
 * Effect: std::make_pair( first_min_element(first, last, comp),
 *                         last_max_element(first, last, comp) );
 *
 * last_min_first_max_element(first, last)
 * Effect: std::make_pair( last_min_element(first, last),
 *                         first_max_element(first, last) );
 *                    
 * last_min_first_max_element(first, last, comp)
 * Effect: std::make_pair( last_min_element(first, last, comp),
 *                         first_max_element(first, last, comp) );
 *
 * last_min_last_max_element(first, last)
 * Effect: std::make_pair( last_min_element(first, last),
 *                         last_max_element(first, last) );
 *                    
 * last_min_last_max_element(first, last, comp)
 * Effect: std::make_pair( last_min_element(first, last, comp),
 *                         last_max_element(first, last, comp) );
 */

namespace boost {

  template <class ForwardIter>
  ForwardIter
  first_min_element(ForwardIter first, ForwardIter last)
  {
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
      if (*first < *min_result)
        min_result = first;
    return min_result;
  }

  template <class ForwardIter, class BinaryPredicate>
  ForwardIter
  first_min_element(ForwardIter first, ForwardIter last,
                    BinaryPredicate comp)
  {
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
      if (comp(*first, *min_result))
        min_result = first;
    return min_result;
  }

  template <class ForwardIter>
  ForwardIter
  last_min_element(ForwardIter first, ForwardIter last)
  {
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
      if (!(*min_result < *first))
        min_result = first;
    return min_result;
  }

  template <class ForwardIter, class BinaryPredicate>
  ForwardIter
  last_min_element(ForwardIter first, ForwardIter last,
                   BinaryPredicate comp)
  {
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
      if (!comp(*min_result, *first))
        min_result = first;
    return min_result;
  }

  template <class ForwardIter>
  ForwardIter
  first_max_element(ForwardIter first, ForwardIter last)
  {
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
      if (*max_result < *first)
        max_result = first;
    return max_result;
  }

  template <class ForwardIter, class BinaryPredicate>
  ForwardIter
  first_max_element(ForwardIter first, ForwardIter last,
                    BinaryPredicate comp)
  {
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
      if (comp(*max_result,*first))
        max_result = first;
    return max_result;
  }

  template <class ForwardIter>
  ForwardIter
  last_max_element(ForwardIter first, ForwardIter last)
  {
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
      if (!(*first < *max_result))
        max_result = first;
    return max_result;
  }

  template <class ForwardIter, class BinaryPredicate>
  ForwardIter
  last_max_element(ForwardIter first, ForwardIter last,
                   BinaryPredicate comp)
  {
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
      if (!comp(*first,*max_result))
        max_result = first;
    return max_result;
  }

  template <class ForwardIter>
  inline std::pair<ForwardIter,ForwardIter>
  first_min_first_max_element(ForwardIter first, ForwardIter last)
  {
    return minmax_element(first, last);
  }

  template <class ForwardIter, class Compare>
  inline std::pair<ForwardIter,ForwardIter>
  first_min_first_max_element(ForwardIter first, ForwardIter last, Compare comp)
  {
    return minmax_element(first, last, comp);
  }

  template <class ForwardIter>
  std::pair<ForwardIter,ForwardIter>
  first_min_last_max_element(ForwardIter first, ForwardIter last)
  {
    // if empty input range
    if (first == last) return std::make_pair(last,last);
    // otherwise two temp variables hold the result
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // if only one element
    ForwardIter second = ++first;
    if (second == last) return std::make_pair(min_result, max_result);
    // treat first pair separately (only one comparison for first two elements)
    if (*second < *min_result) min_result = second;
    else                       max_result = second;
    // treat elements by pairs
    first = ++second; if (first != last) ++second;
    while (second != last) {
      if (!(*second < *first)) {
        if (*first < *min_result)      min_result = first;
        if (!(*second < *max_result))  max_result = second;
      } else {
        if (*second < *min_result)     min_result = second;
        if (!(*first < *max_result))   max_result = first;
      }
      first = ++second; if (first != last) ++second;
    }
    // if the number of elements is odd
    if (first != last) {
      if (*first < *min_result)         min_result = first;
      else if (!(*first < *max_result)) max_result = first;
    }
    return std::make_pair(min_result, max_result);
  }

  template <class ForwardIter, class BinaryPredicate>
  std::pair<ForwardIter,ForwardIter>
  first_min_last_max_element(ForwardIter first, ForwardIter last,
                             BinaryPredicate comp)
  {
    // if empty input range
    if (first == last) return std::make_pair(last,last);
    // otherwise two temp variables hold the result
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // if only one element
    ForwardIter second = ++first;
    if (second == last) return std::make_pair(min_result, max_result);
    // treat first pair separately (only one comparison for first two elements)
    if (comp(*second, *min_result)) min_result = second;
    else                             max_result = second;
    // treat elements by pairs
    first = ++second; if (first != last) ++second;
    while (second != last) {
      if (!comp(*second, *first)) {
        if (comp(*first, *min_result))   min_result = first;
        if (!comp(*second, *max_result)) max_result = second;
      } else {
        if (comp(*second, *min_result))  min_result = second;
        if (!comp(*first, *max_result))      max_result = first;
      }
      first = ++second; if (first != last) ++second;
    }
    // if the number of elements is odd
    if (first != last) {
      if (comp(*first, *min_result))       min_result = first;
      else if (!comp(*first, *max_result)) max_result = first;
    }
    return std::make_pair(min_result, max_result);
  }

  template <class ForwardIter>
  std::pair<ForwardIter,ForwardIter>
  last_min_first_max_element(ForwardIter first, ForwardIter last)
  {
    // if empty input range
    if (first == last) return std::make_pair(last,last);
    // otherwise two temp variables hold the result
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // if only one element
    ForwardIter second = ++first;
    if (second == last) return std::make_pair(min_result, max_result);
    // treat first pair separately (only one comparison for first two elements)
    if (*max_result < *second) max_result = second;
    else                       min_result = second;
    // treat elements by pairs
    first = ++second; if (first != last) ++second;
    while (second != last)  {
      if (*first < *second) {
        if (!(*min_result < *first))   min_result = first;
        if (*max_result < *second)     max_result = second;
      } else {
        if (!(*min_result < *second))  min_result = second;
        if (*max_result < *first)      max_result = first;
      }
      first = ++second; if (first != last) ++second;
    }
    // if the number of elements is odd, treat last element
    if (first != last) {
      if (!(*min_result < *first))    min_result = first;
      else if (*max_result < *first)  max_result = first;
    }
    return std::make_pair(min_result, max_result);
  }

  template <class ForwardIter, class BinaryPredicate>
  std::pair<ForwardIter,ForwardIter>
  last_min_first_max_element(ForwardIter first, ForwardIter last,
                             BinaryPredicate comp)
  {
    // if empty input range
    if (first == last) return std::make_pair(last,last);
    // otherwise two temp variables hold the result
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // if only one element
    ForwardIter second = ++first;
    if (second == last) return std::make_pair(min_result, max_result);
    // treat first pair separately (only one comparison for first two elements)
    if (comp(*max_result, *second)) max_result = second;
    else                            min_result = second;
    // treat elements by pairs
    first = ++second; if (first != last) ++second;
    while (second != last)  {
      if (comp(*first, *second)) {
        if (!comp(*min_result, *first))   min_result = first;
        if (comp(*max_result, *second))   max_result = second;
      } else {
        if (!comp(*min_result, *second)) min_result = second;
        if (comp(*max_result, *first))   max_result = first;
      }
      first = ++second; if (first != last) ++second;
    }
    // if the number of elements is odd, treat last element
    if (first != last) {
      if (!comp(*min_result, *first))    min_result = first;
      else if (comp(*max_result, *first))  max_result = first;
    }
    return std::make_pair(min_result, max_result);
  }

  template <class ForwardIter>
  std::pair<ForwardIter,ForwardIter>
  last_min_last_max_element(ForwardIter first, ForwardIter last)
  {
    if (first == last) return std::make_pair(last,last);
    // if only one element
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // one element
    ForwardIter second = first; ++second;
    if (second == last) return std::make_pair(min_result,max_result);
    // treat first pair with at most 1 comparison
    ForwardIter potential_max_result = last;
    if (*first < *second)
      max_result = second;
    else
      min_result = second, potential_max_result = second;
    // then each element by pairs, with at most 3 comparisons per pair
    first = ++second; if (first != last) ++second;
    while (second != last) {
      if ((*first < *second)) {
        if (!(*min_result < *first))
	  min_result = first;
	if (!(*second < *max_result))
	  max_result = second, potential_max_result = last;
      } else {
	// could be that *first == *second
        if (!(*min_result < *second))
	  min_result = second;
	if (!(*first < *max_result))
	  max_result = first, potential_max_result = second;
      }
      first = ++second; if (first != last) ++second;
    }
    // if odd number of elements, treat last element with 2 comparisons
    if (first != last) {
      if (!(*min_result < *first))
	min_result = first;
      else if (!(*first < *max_result))
	max_result = first, potential_max_result = last;
    }
    // resolve max_result being incorrect, with one extra comparison
    // (in which case potential_max_result is necessarily the correct result)
    if (potential_max_result != last
        && !(*potential_max_result < *max_result))
      max_result = potential_max_result;
    return std::make_pair(min_result,max_result);
  }

  template <class ForwardIter, class BinaryPredicate>
  std::pair<ForwardIter,ForwardIter>
  last_min_last_max_element(ForwardIter first, ForwardIter last,
                            BinaryPredicate comp)
  {
    if (first == last) return std::make_pair(last,last);
    // if only one element
    ForwardIter min_result = first;
    ForwardIter max_result = first;
    // one element
    ForwardIter second = first; ++second;
    if (second == last) return std::make_pair(min_result,max_result);
    // treat first pair with at most 1 comparison
    ForwardIter potential_max_result = last;
    if (comp(*first, *second))
      max_result = second;
    else
      min_result = second, potential_max_result = second;
    // then each element by pairs, with at most 3 comparisons per pair
    first = ++second; if (first != last) ++second;
    while (second != last) {
      if (comp(*first, *second)) {
        if (!comp(*min_result, *first))
	  min_result = first;
	if (!comp(*second, *max_result))
	  max_result = second, potential_max_result = last;
      } else {
	// could be that *first == *second
        if (!comp(*min_result, *second))
	  min_result = second;
	if (!comp(*first, *max_result))
	  max_result = first, potential_max_result = second;
      }
      first = ++second; if (first != last) ++second;
    }
    // if odd number of elements, treat last element with 2 comparisons
    if (first != last) {
      if (!comp(*min_result, *first))
	min_result = first;
      else if (!comp(*first, *max_result))
	max_result = first, potential_max_result = last;
    }
    // resolve max_result being incorrect, with one extra comparison
    // (in which case potential_max_result is necessarily the correct result)
    if (potential_max_result != last
        && !comp(*potential_max_result, *max_result))
      max_result = potential_max_result;
    return std::make_pair(min_result,max_result);
  }

} // namespace boost

#endif // #ifdef BOOST_MINMAX_H
