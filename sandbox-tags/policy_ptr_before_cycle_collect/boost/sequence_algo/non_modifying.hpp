/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * Copyright (c) 2002 
 * Jeremy Siek
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef BOOST_SEQUENCE_ALGO_NON_MODIFYING_HPP
#define BOOST_SEQUENCE_ALGO_NON_MODIFYING_HPP

#include "boost/iterator/iterator_concepts.hpp"
#include "boost/iterator/iterator_categories.hpp"
#include "boost/functional_mem_tmpl.hpp"
#include "boost/concept_check.hpp"
#include "boost/sequence_algo/detail/non_modifying.hpp"

/*  Placing this in namespace boost causes a lot of problem.
    The C++ std lib implementation sometimes uses unqualified
    calls to 'find'. Therefore, if it calls find with functional
    object from boost:: namespace, the 'find' below will be found
    in addition to std::find, and the call will be ambiguous. 
*/
namespace boost { namespace algorith { namespace non_modifying {
  
  template <typename Iterator, typename Function>
  Function for_each(Iterator first, Iterator last, Function f)
  {
    function_requires< InputIteratorConcept<Iterator> >();
    typedef typename std::iterator_traits<Iterator>::value_type T;
    function_requires< UnaryFunctionConcept<Function, void, T> >();
    for ( ; first != last; ++first)
      f(*first);
    return f;
  }

  template <typename Iterator, typename T>
  Iterator find(Iterator first, Iterator last, const T& val)
  {
    function_requires< InputIteratorConcept<Iterator> >();
    typedef typename std::iterator_traits<Iterator>::value_type VT;
    function_requires< EqualOpConcept<VT,T> >();
    typename traversal_category<Iterator>::type traversal;
    return detail::find(first, last, val, traversal);
  }
  
  template <typename Iterator, typename Predicate>
  Iterator find_if(Iterator first, Iterator last, Predicate pred) {
    typename traversal_category<Iterator>::type traversal;
    return detail::find_if(first, last, pred, traversal);
  }
  
  template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
  Iterator1 
  find_end(Iterator1 first1, Iterator1 last1, 
	   Iterator2 first2, Iterator2 last2,
	   BinaryPredicate comp)
  {
    using namespace boost_concepts;
    function_requires< ReadableIteratorConcept<Iterator1> >();
    function_requires< ForwardTraversalConcept<Iterator1> >();
    function_requires< ReadableIteratorConcept<Iterator2> >();
    function_requires< ForwardTraversalConcept<Iterator2> >();
    typedef typename std::iterator_traits<Iterator1>::value_type T1;
    typedef typename std::iterator_traits<Iterator2>::value_type T2;
    function_requires< BinaryPredicateConcept<BinaryPredicate, T1, T2> >();

    typename traversal_category<Iterator1>::type traversal1;
    typename traversal_category<Iterator2>::type traversal2;
    return detail::find_end
      (first1, last1, first2, last2, traversal1, traversal2, comp);
  }

  template <typename Iterator1, typename Iterator2>
  Iterator1
  find_end(Iterator1 first1, Iterator1 last1, 
	   Iterator2 first2, Iterator2 last2)
  {
    using namespace boost_concepts;
    function_requires< ReadableIteratorConcept<Iterator1> >();
    function_requires< ForwardTraversalConcept<Iterator1> >();
    function_requires< ReadableIteratorConcept<Iterator2> >();
    function_requires< ForwardTraversalConcept<Iterator2> >();
    typedef typename std::iterator_traits<Iterator1>::value_type T1;
    typedef typename std::iterator_traits<Iterator2>::value_type T2;
    function_requires< EqualOpConcept<T1,T2> >();

    typename traversal_category<Iterator1>::type traversal1;
    typename traversal_category<Iterator2>::type traversal2;
    return detail::find_end
      (first1, last1, first2, last2, traversal1, traversal2, boost::equal_to());
  }

  template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
  Iterator1 find_first_of(Iterator1 first1, Iterator1 last1,
			  Iterator2 first2, Iterator2 last2,
			  BinaryPredicate comp)
  {
    using namespace boost_concepts;
    function_requires< InputIteratorConcept<Iterator1> >();
    function_requires< ReadableIteratorConcept<Iterator2> >();
    function_requires< ForwardTraversalConcept<Iterator2> >();
    typedef typename std::iterator_traits<Iterator1>::value_type T1;
    typedef typename std::iterator_traits<Iterator2>::value_type T2;
    function_requires< BinaryPredicateConcept<BinaryPredicate, T1, T2> >();

    for ( ; first1 != last1; ++first1) 
      for (Iterator2 iter = first2; iter != last2; ++iter)
	if (comp(*first1, *iter))
	  return first1;
    return last1;
  }
  
  template <typename Iterator1, typename Iterator2>
  inline Iterator1 find_first_of(Iterator1 first1, Iterator1 last1,
				 Iterator2 first2, Iterator2 last2) {
    return find_first_of(first1, last1, first2, last2, boost::equal_to());
  }
  
  template <typename Iterator>
  Iterator adjacent_find(Iterator first, Iterator last)
  {
    using namespace boost_concepts;
    function_requires< ReadableIteratorConcept<Iterator> >();
    function_requires< ForwardTraversalConcept<Iterator> >();
    typedef typename std::iterator_traits<Iterator>::value_type T;
    function_requires< EqualOpConcept<T, T> >();
    
    if (first == last)
      return last;
    Iterator next = first;
    while(++next != last) {
      if (*first == *next)
	return first;
      first = next;
    }
    return last;
  }

  template <typename Iterator, typename BinaryPredicate>
  Iterator 
  adjacent_find(Iterator first, Iterator last, BinaryPredicate binary_pred)
  {
    using namespace boost_concepts;
    function_requires< ReadableIteratorConcept<Iterator> >();
    function_requires< ForwardTraversalConcept<Iterator> >();
    typedef typename std::iterator_traits<Iterator>::value_type T;
    function_requires< BinaryPredicateConcept<BinaryPredicate, T, T> >();
    
    if (first == last)
      return last;
    Iterator next = first;
    while(++next != last) {
      if (binary_pred(*first, *next))
	return first;
      first = next;
    }
    return last;
  }



  template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
  Iterator1 search(Iterator1 first1, Iterator1 last1,
		   Iterator2 first2, Iterator2 last2,
		   BinaryPredicate  predicate) 
  {
    using namespace boost_concepts;
    function_requires< ReadableIteratorConcept<Iterator1> >();
    function_requires< ForwardTraversalConcept<Iterator1> >();
    function_requires< ReadableIteratorConcept<Iterator2> >();
    function_requires< ForwardTraversalConcept<Iterator2> >();
    typedef typename std::iterator_traits<Iterator1>::value_type T1;
    typedef typename std::iterator_traits<Iterator2>::value_type T2;
    function_requires< BinaryPredicateConcept<BinaryPredicate, T1, T2> >();    
    detail::search(first1, last1, first2, last2, predicate);
  }  
  
}}} // namespace boost::algorithm::non_modifying


#endif // BOOST_SEQUENCE_ALGO_NON_MODIFYING_HPP
