#ifndef BOOST_SEQUENCE_ALGO_DETAIL_NON_MODIFYING_HPP
#define BOOST_SEQUENCE_ALGO_DETAIL_NON_MODIFYING_HPP

#include "boost/iterator/iterator_categories.hpp"
#include "boost/iterator_adaptors.hpp" // for reverse_iterator_generator
                                       // replace with new iterator adaptors

namespace boost {
  namespace detail {

    template <typename Iterator, typename T>
    Iterator find(Iterator first, Iterator last, const T& val,
                  const random_access_traversal_tag&)
    {
      typename std::iterator_traits<Iterator>::difference_type
        trip_count = (last - first) >> 2;

      for ( ; trip_count > 0 ; --trip_count) {
        if (*first == val) return first;
        ++first;

        if (*first == val) return first;
        ++first;

        if (*first == val) return first;
        ++first;

        if (*first == val) return first;
        ++first;
      }
      switch(last - first) {
      case 3:
        if (*first == val) return first;
        ++first;
      case 2:
        if (*first == val) return first;
        ++first;
      case 1:
        if (*first == val) return first;
        ++first;
      case 0:
      default:
        return last;
      }
    }

    template <typename Iterator, typename T>
    inline Iterator find(Iterator first, Iterator last, const T& val,
                         const input_traversal_tag&)
    {
      while (first != last && !(*first == val))
        ++first;
      return first;
    }

    template <class Iterator, class Predicate>
    Iterator find_if(Iterator first, Iterator last, Predicate pred,
		     const random_access_traversal_tag&)
    {
      typename std::iterator_traits<Iterator>::difference_type
	trip_count = (last - first) >> 2;
      
      for ( ; trip_count > 0 ; --trip_count) {
	if (pred(*first)) return first;
	++first;

	if (pred(*first)) return first;
	++first;

	if (pred(*first)) return first;
	++first;

	if (pred(*first)) return first;
	++first;
      }
      switch(last - first) {
      case 3:
	if (pred(*first)) return first;
	++first;
      case 2:
	if (pred(*first)) return first;
	++first;
      case 1:
	if (pred(*first)) return first;
      case 0:
      default:
	return last;
      }
    }

    template <class Iterator, class Predicate>
    inline Iterator find_if(Iterator first, Iterator last, Predicate pred,
			    const input_traversal_tag&)
    {
      while (first != last && !pred(*first))
	++first;
      return first;
    }

    template <class Iterator1, class Iterator2, class BinaryPredicate>
    Iterator1 search(Iterator1 first1, Iterator1 last1,
		     Iterator2 first2, Iterator2 last2,
		     BinaryPredicate  predicate) 
    {
	// Test for empty ranges
	if (first1 == last1 || first2 == last2)
	  return first1;

      // Test for a pattern of length 1.
      Iterator2 tmp(first2);
      ++tmp;
      if (tmp == last2) {
	while (first1 != last1 && !predicate(*first1, *first2))
	  ++first1;
	return first1;    
      }

      // General case.

      Iterator2 p1, p;

      p1 = first2; ++p1;

      //  Iterator1 current = first1;

      while (first1 != last1) {
	while (first1 != last1) {
	  if (predicate(*first1, *first2))
	    break;
	  ++first1;
	}
	while (first1 != last1 && !predicate(*first1, *first2))
	  ++first1;
	if (first1 == last1)
	  return last1;

	p = p1;
	Iterator1 current = first1; 
	if (++current == last1) return last1;

	while (predicate(*current, *p)) {
	  if (++p == last2)
	    return first1;
	  if (++current == last1)
	    return last1;
	}

	++first1;
      }
      return first1;
    }
    


    template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
    Iterator1 find_end(Iterator1 first1, Iterator1 last1,
                       Iterator2 first2, Iterator2 last2,
                       const forward_traversal_tag&, 
                       const forward_traversal_tag&,
                       BinaryPredicate comp)
    {
      if (first2 == last2)
        return last1;
      else {
        Iterator1 result = last1;
        while (1) {
          Iterator1 new_result
            = search(first1, last1, first2, last2, comp);
          if (new_result == last1)
            return result;
          else {
            result = new_result;
            first1 = new_result;
            ++first1;
          }
        }
      }
    }

    template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
    Iterator1
    find_end(Iterator1 first1, Iterator1 last1,
             Iterator2 first2, Iterator2 last2,
             const bidirectional_traversal_tag&, 
             const bidirectional_traversal_tag&, 
             BinaryPredicate comp)
    {
      typedef typename reverse_iterator_generator<Iterator1>::type RevIter1;
      typedef typename reverse_iterator_generator<Iterator2>::type RevIter2;

      RevIter1 rlast1(first1);
      RevIter2 rlast2(first2);
      RevIter1 rresult = search(RevIter1(last1), rlast1,
                                RevIter2(last2), rlast2,
                                comp);
      
      if (rresult == rlast1)
        return last1;
      else {
        Iterator1 result = rresult.base();
        advance(result, -distance(first2, last2));
        return result;
      }
    }

  } // namespace detail
} // namespace boost

#endif // BOOST_SEQUENCE_ALGO_DETAIL_NON_MODIFYING_HPP
