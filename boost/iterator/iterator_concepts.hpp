// (C) Copyright Jeremy Siek 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_ITERATOR_CONCEPTS_HPP
#define BOOST_ITERATOR_CONCEPTS_HPP

#include <boost/concept_check.hpp>
#include <boost/iterator_categories.hpp>
#include <boost/type_traits/conversion_traits.hpp>
#include <boost/static_assert.hpp>

namespace boost_concepts {
  // Used a different namespace here (instead of "boost") so that the
  // concept descriptions do not take for granted the names in
  // namespace boost.


  //===========================================================================
  // Iterator Access Concepts

  template <typename Iterator>
  class ReadableIteratorConcept {
  public:
    typedef typename std::iterator_traits<Iterator>::value_type value_type;
    typedef typename std::iterator_traits<Iterator>::reference reference;
    typedef typename boost::return_category<Iterator>::type return_category;

    void constraints() {
      boost::function_requires< boost::SGIAssignableConcept<Iterator> >();
      boost::function_requires< boost::EqualityComparableConcept<Iterator> >();
      boost::function_requires< 
        boost::DefaultConstructibleConcept<Iterator> >();
      
      BOOST_STATIC_ASSERT((boost::is_convertible<return_category*,
                           boost::readable_iterator_tag*>::value));
                          
      reference r = *i; // or perhaps read(x)
      value_type v(r);
      boost::ignore_unused_variable_warning(v);
    }
    Iterator i;
  };
  
  template <typename Iterator, typename ValueType>
  class WritableIteratorConcept {
  public:
    typedef typename boost::return_category<Iterator>::type return_category;

    void constraints() {
      boost::function_requires< boost::SGIAssignableConcept<Iterator> >();
      boost::function_requires< boost::EqualityComparableConcept<Iterator> >();
      boost::function_requires< 
        boost::DefaultConstructibleConcept<Iterator> >();
      
      BOOST_STATIC_ASSERT((boost::is_convertible<return_category*,
                           boost::writable_iterator_tag*>::value));

      *i = v; // a good alternative could be something like write(x, v)
    }
    ValueType v;
    Iterator i;
  };
  
  template <typename Iterator>
  class ConstantLvalueIteratorConcept {
  public:
    typedef typename std::iterator_traits<Iterator>::value_type value_type;
    typedef typename std::iterator_traits<Iterator>::reference reference;
    typedef typename boost::return_category<Iterator>::type return_category;

    void constraints() {
      boost::function_requires< ReadableIteratorConcept<Iterator> >();

      BOOST_STATIC_ASSERT((boost::is_convertible<return_category*, 
                           boost::constant_lvalue_iterator_tag*>::value));

      BOOST_STATIC_ASSERT((boost::is_same<reference, 
                           const value_type&>::value));

      reference v = *i;
      boost::ignore_unused_variable_warning(v);
    }
    Iterator i;
  };

  template <typename Iterator>
  class MutableLvalueIteratorConcept {
  public:
    typedef typename std::iterator_traits<Iterator>::value_type value_type;
    typedef typename std::iterator_traits<Iterator>::reference reference;
    typedef typename boost::return_category<Iterator>::type return_category;

    void constraints() {
      boost::function_requires< ReadableIteratorConcept<Iterator> >();
      boost::function_requires< 
        WritableIteratorConcept<Iterator, value_type> >();
      
      BOOST_STATIC_ASSERT((boost::is_convertible<return_category*, 
                           boost::mutable_lvalue_iterator_tag*>::value));

      BOOST_STATIC_ASSERT((boost::is_same<reference, value_type&>::value));

      reference v = *i;
      boost::ignore_unused_variable_warning(v);
    }
    Iterator i;
  };
  
  //===========================================================================
  // Iterator Traversal Concepts

  template <typename Iterator>
  class ForwardIteratorConcept {
  public:
    typedef typename boost::traversal_category<Iterator>::type traversal_category;

    void constraints() {
      boost::function_requires< boost::SGIAssignableConcept<Iterator> >();
      boost::function_requires< boost::EqualityComparableConcept<Iterator> >();
      boost::function_requires< 
        boost::DefaultConstructibleConcept<Iterator> >();

      BOOST_STATIC_ASSERT((boost::is_convertible<traversal_category*, 
                           boost::forward_traversal_tag*>::value));
      ++i;
      (void)i++;
    }
    Iterator i;
  };
  
  template <typename Iterator>
  class BidirectionalIteratorConcept {
  public:
    typedef typename boost::traversal_category<Iterator>::type traversal_category;

    void constraints() {
      boost::function_requires< ForwardIteratorConcept<Iterator> >();
      
      BOOST_STATIC_ASSERT((boost::is_convertible<traversal_category*, 
                           boost::bidirectional_traversal_tag*>::value));

      --i;
      (void)i--;
    }
    Iterator i;
  };

  template <typename Iterator>
  class RandomAccessIteratorConcept {
  public:
    typedef typename boost::traversal_category<Iterator>::type traversal_category;
    typedef typename std::iterator_traits<Iterator>::difference_type
      difference_type;

    void constraints() {
      boost::function_requires< BidirectionalIteratorConcept<Iterator> >();

      BOOST_STATIC_ASSERT((boost::is_convertible<traversal_category*, 
                           boost::random_access_traversal_tag*>::value));
      
      i += n;
      i = i + n;
      i = n + i;
      i -= n;
      i = i - n;
      n = i - j;
    }
    difference_type n;
    Iterator i, j;
  };

} // namespace boost_concepts


#endif // BOOST_ITERATOR_CONCEPTS_HPP
