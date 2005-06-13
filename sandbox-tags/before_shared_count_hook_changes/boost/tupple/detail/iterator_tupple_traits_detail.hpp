
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_TRAITS_DETAIL_HPP
#define BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_TRAITS_DETAIL_HPP

#include <boost/iterator/iterator_traits.hpp>


namespace boost {
  namespace tupple {
    namespace detail {

/**
 * @brief Generates types in much the same way as iterator_adaptor does.
 */
template<class IteratorT,
         class ValueT       = typename ::boost::iterator_value<      IteratorT >::type,
         class ReferenceT   = typename ::boost::iterator_reference<  IteratorT >::type,
         class PointerT     = typename ::boost::iterator_pointer<    IteratorT >::type,
         class CategoryT    = typename ::boost::iterator_category<   IteratorT >::type,
         class DifferenceT  = typename ::boost::iterator_difference< IteratorT >::type >
struct iterator_traits {

  /// The value type of the iterator.
  typedef ValueT           value_type;

  /// The reference type of the iterator.
  typedef ReferenceT       reference;
  /// The reference type of the const iterator.
  typedef ::boost::add_const< ReferenceT >  const_reference;
  // Correct?
  // In connection with tuples:
  // const tuple<A&,B&> is NOT the same as tuple<const A&,const B&> !

  /// The pointer type of the iterator.
  typedef PointerT         pointer;
  /// The pointer type of the const iterator.
  //typedef const pointer    const_pointer;

  /// The iterator type.
  typedef IteratorT       iterator;

  /// The const iterator type (questionable).
  typedef ::boost::add_const< IteratorT > const_iterator;

  /// The distance type of the iterator.
  typedef DifferenceT     difference_type;

  /// The iterator category of the iterator.
  typedef CategoryT       iterator_category;
};



} // namespace detail
} // namespace tupple
} // namespace boost

#endif
