//  Test boost/iterator_adaptors.hpp

//  (C) Copyright Jeremy Siek 1999. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  19 Feb 01 Take adavantage of improved iterator_traits to do more tests
//            on MSVC. Hack around an MSVC-with-STLport internal compiler
//            error. (David Abrahams)
//  11 Feb 01 Added test of operator-> for forward and input iterators.
//            (Jeremy Siek)
//  11 Feb 01 Borland fixes (David Abrahams)
//  10 Feb 01 Use new adaptors interface. (David Abrahams)
//  10 Feb 01 Use new filter_ interface. (David Abrahams)
//  09 Feb 01 Use new reverse_ and indirect_ interfaces. Replace
//            BOOST_NO_STD_ITERATOR_TRAITS with
//            BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION to prove we've
//            normalized to core compiler capabilities (David Abrahams)
//  08 Feb 01 Use Jeremy's new make_reverse_iterator form; add more
//            comprehensive testing. Force-decay array function arguments to
//            pointers.
//  07 Feb 01 Added tests for the make_xxx_iterator() helper functions.
//            (Jeremy Siek)
//  07 Feb 01 Replaced use of xxx_pair_generator with xxx_generator where
//            possible (which was all but the projection iterator).
//            (Jeremy Siek)
//  06 Feb 01 Removed now-defaulted template arguments where possible
//            Updated names to correspond to new generator naming convention.
//            Added a trivial test for make_transform_iterator().
//            Gave traits for const iterators a mutable value_type, per std.
//            Resurrected my original tests for indirect iterators.
//            (David Abrahams)
//  04 Feb 01 Fix for compilers without standard iterator_traits
//            (David Abrahams)
//  13 Jun 00 Added const version of the iterator tests (Jeremy Siek)
//  12 Dec 99 Initial version with iterator operators (Jeremy Siek)

#include <boost/config.hpp>
#include <iostream>

#include <algorithm>
#include <functional>

#include <boost/iterator_adaptors.hpp>
#include <boost/pending/iterator_tests.hpp>
#include <boost/pending/integer_range.hpp>
#include <boost/concept_archetype.hpp>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <set>

struct my_iterator_tag : public std::random_access_iterator_tag { };

using boost::dummyT;


struct mult_functor {
  typedef int result_type;
  typedef int argument_type;
  // Functors used with transform_iterator must be
  // DefaultConstructible, as the transform_iterator must be
  // DefaultConstructible to satisfy the requirements for
  // TrivialIterator.
  mult_functor() { }
  mult_functor(int aa) : a(aa) { }
  int operator()(int b) const { return a * b; }
  int a;
};

template <class Pair>
struct select1st_ 
  : public std::unary_function<Pair, typename Pair::first_type>
{
  const typename Pair::first_type& operator()(const Pair& x) const {
    return x.first;
  }
  typename Pair::first_type& operator()(Pair& x) const {
    return x.first;
  }
};

struct one_or_four {
  bool operator()(dummyT x) const {
    return x.foo() == 1 || x.foo() == 4;
  }
};

typedef std::deque<int> storage;
typedef std::deque<int*> pointer_deque;
typedef std::set<storage::iterator> iterator_set;

int
main()
{
  dummyT array[] = { dummyT(0), dummyT(1), dummyT(2), 
                     dummyT(3), dummyT(4), dummyT(5) };
  const int N = sizeof(array)/sizeof(dummyT);

  // sanity check, if this doesn't pass the test is buggy
  boost::random_access_iterator_test(array,N,array);

  // Check that the policy concept checks and the default policy
  // implementation match up.
  boost::function_requires< 
     boost::RandomAccessIteratorPoliciesConcept<
       boost::default_iterator_policies, int*,
       boost::iterator<std::random_access_iterator_tag, int, std::ptrdiff_t,
                      int*, int&>
      > >();

  // Test the iterator_adaptor
  {
    boost::iterator_adaptor<dummyT*, boost::default_iterator_policies, dummyT> i(array);
    boost::random_access_iterator_test(i, N, array);
    
    boost::iterator_adaptor<const dummyT*, boost::default_iterator_policies, const dummyT> j(array);
    boost::random_access_iterator_test(j, N, array);
    boost::const_nonconst_iterator_test(i, ++j);
  }

  // Test projection_iterator_pair_generator
  {    
    typedef std::pair<dummyT,dummyT> Pair;
    Pair pair_array[N];
    for (int k = 0; k < N; ++k)
      pair_array[k].first = array[k];

    typedef boost::projection_iterator_pair_generator<select1st_<Pair>,
      Pair*, const Pair*
      > Projection;
    
    Projection::iterator i(pair_array);
    boost::random_access_iterator_test(i, N, array);

    boost::random_access_iterator_test(boost::make_projection_iterator(pair_array, select1st_<Pair>()), N, array);    
    boost::random_access_iterator_test(boost::make_projection_iterator< select1st_<Pair> >(pair_array), N, array);    

    Projection::const_iterator j(pair_array);
    boost::random_access_iterator_test(j, N, array);

    boost::random_access_iterator_test(boost::make_const_projection_iterator(pair_array, select1st_<Pair>()), N, array);
    boost::random_access_iterator_test(boost::make_const_projection_iterator<select1st_<Pair> >(pair_array), N, array);

    boost::const_nonconst_iterator_test(i, ++j);
  }

  // Test reverse_iterator_generator
  {
    dummyT reversed[N];
    std::copy(array, array + N, reversed);
    std::reverse(reversed, reversed + N);
    
    typedef boost::reverse_iterator_generator<dummyT*
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        , dummyT
#endif
      >::type reverse_iterator;
    
    reverse_iterator i(reversed + N);
    boost::random_access_iterator_test(i, N, array);

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    boost::random_access_iterator_test(boost::make_reverse_iterator(reversed + N), N, array);
#endif

    typedef boost::reverse_iterator_generator<const dummyT*
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        , const dummyT
#endif
      >::type const_reverse_iterator;
    
    const_reverse_iterator j(reversed + N);
    boost::random_access_iterator_test(j, N, array);

    const dummyT* const_reversed = reversed;
    
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    boost::random_access_iterator_test(boost::make_reverse_iterator(const_reversed + N), N, array);
#endif
    
    boost::const_nonconst_iterator_test(i, ++j);    
  }

  // Test reverse_iterator_generator again, with traits fully deducible on all platforms
  {
    std::deque<dummyT> reversed_container;
    std::reverse_copy(array, array + N, std::back_inserter(reversed_container));
    const std::deque<dummyT>::iterator reversed = reversed_container.begin();


    typedef boost::reverse_iterator_generator<
        std::deque<dummyT>::iterator>::type reverse_iterator;
    typedef boost::reverse_iterator_generator<
        std::deque<dummyT>::const_iterator, const dummyT>::type const_reverse_iterator;

    // MSVC/STLport gives an INTERNAL COMPILER ERROR when any computation
    // (e.g. "reversed + N") is used in the constructor below.
    const std::deque<dummyT>::iterator finish = reversed_container.end();
    reverse_iterator i(finish);
    
    boost::random_access_iterator_test(i, N, array);
    boost::random_access_iterator_test(boost::make_reverse_iterator(reversed + N), N, array);

    const_reverse_iterator j = reverse_iterator(finish);
    boost::random_access_iterator_test(j, N, array);

    const std::deque<dummyT>::const_iterator const_reversed = reversed;
    boost::random_access_iterator_test(boost::make_reverse_iterator(const_reversed + N), N, array);
    
    // Many compilers' builtin deque iterators don't interoperate well, though
    // STLport fixes that problem.
#if defined(__SGI_STL_PORT) || !defined(__GNUC__) && !defined(__BORLANDC__) && !defined(BOOST_MSVC)
    boost::const_nonconst_iterator_test(i, ++j);
#endif
  }
  
  // Test integer_range's iterators
  {
    int int_array[] = { 0, 1, 2, 3, 4, 5 };
    boost::integer_range<int> r(0, 5);
    boost::random_access_iterator_test(r.begin(), r.size(), int_array);
  }

  // Test filter iterator
  {
    // Using typedefs for filter_gen::type confused Borland terribly.
    typedef boost::detail::non_bidirectional_category<dummyT*>::type category;
    
    typedef boost::filter_iterator_generator<one_or_four, dummyT*
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        , dummyT
#endif
        >::type filter_iter;

#if defined(__BORLANDC__)
    // Borland is choking on accessing the policies_type explicitly
    // from the filter_iter. 
    boost::forward_iterator_test(make_filter_iterator(array, array+N, 
						      one_or_four()),
				 dummyT(1), dummyT(4));
#else
    filter_iter i(array, filter_iter::policies_type(one_or_four(), array + N));
    boost::forward_iterator_test(i, dummyT(1), dummyT(4));
#endif

#if !defined(__BORLANDC__)
    // 
    enum { is_forward = boost::is_same<
           filter_iter::iterator_category,
           std::forward_iterator_tag>::value };
    BOOST_STATIC_ASSERT(is_forward);
#endif

    // On compilers not supporting partial specialization, we can do more type
    // deduction with deque iterators than with pointers... unless the library
    // is broken ;-(
#if !defined(BOOST_MSVC) || defined(__SGI_STL_PORT)
    std::deque<dummyT> array2;
    std::copy(array+0, array+N, std::back_inserter(array2));
    boost::forward_iterator_test(
        boost::make_filter_iterator(array2.begin(), array2.end(), one_or_four()),
        dummyT(1), dummyT(4));

    boost::forward_iterator_test(
        boost::make_filter_iterator<one_or_four>(array2.begin(), array2.end()),
        dummyT(1), dummyT(4));
#endif

#if !defined(BOOST_MSVC) // This just freaks MSVC out completely
    boost::forward_iterator_test(
        boost::make_filter_iterator<one_or_four>(
            boost::make_reverse_iterator(array2.end()),
            boost::make_reverse_iterator(array2.begin())
            ),
        dummyT(4), dummyT(1));
#endif
    
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    boost::forward_iterator_test(
        boost::make_filter_iterator(array+0, array+N, one_or_four()),
        dummyT(1), dummyT(4));

    boost::forward_iterator_test(
        boost::make_filter_iterator<one_or_four>(array, array + N),
        dummyT(1), dummyT(4));

#endif
  }

  // check operator-> with a forward iterator
  {
    boost::forward_iterator_archetype<dummyT> forward_iter;
#if defined(__BORLANDC__)
    typedef boost::iterator_adaptor<boost::forward_iterator_archetype<dummyT>,
      boost::default_iterator_policies,
      dummyT, const dummyT&, const dummyT*, 
      std::forward_iterator_tag, std::ptrdiff_t> adaptor_type;
#else
    typedef boost::iterator_adaptor<boost::forward_iterator_archetype<dummyT>,
      boost::default_iterator_policies,
      boost::iterator_traits_generator
        ::value_type<dummyT>
        ::reference<const dummyT&>
        ::pointer<const dummyT*> 
        ::iterator_category<std::forward_iterator_tag>
        ::difference_type<std::ptrdiff_t> > adaptor_type;
#endif
    adaptor_type i(forward_iter);
    int zero = 0;
    if (zero) // don't do this, just make sure it compiles
      assert((*i).m_x == i->foo());      
  }
  // check operator-> with an input iterator
  {
    boost::input_iterator_archetype<dummyT> input_iter;
    typedef boost::iterator_adaptor<boost::input_iterator_archetype<dummyT>,
      boost::default_iterator_policies,
      dummyT, const dummyT&, const dummyT*, 
      std::input_iterator_tag, std::ptrdiff_t> adaptor_type;
    adaptor_type i(input_iter);
    int zero = 0;
    if (zero) // don't do this, just make sure it compiles
      assert((*i).m_x == i->foo());      
  }
  std::cout << "test successful " << std::endl;
  return 0;
}
