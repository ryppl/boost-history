
// Copyright (C) 2001, 2002 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef CONTAINER_TUPPLE_TRAITS_DETAIL_HPP
#define CONTAINER_TUPPLE_TRAITS_DETAIL_HPP

#include <boost/config.hpp>
#include <boost/detail/iterator.hpp>

namespace boost {
namespace tupple {

namespace detail {

/**
 * @brief Generates types in much the same way as iterator_adaptor does.
 */
template<class IteratorT,
         class ValueT       = boost::detail::iterator_traits<IteratorT>::value_type,
         class ReferenceT   = boost::detail::iterator_traits<IteratorT>::reference,
         class PointerT     = boost::detail::iterator_traits<IteratorT>::pointer,
         class CategoryT    = boost::detail::iterator_traits<IteratorT>::iterator_category,
         class DifferenceT  = boost::detail::iterator_traits<IteratorT>::difference_type>
struct iterator_traits {

  /// The value type of the iterator.
  typedef ValueT           value_type;

  /// The reference type of the iterator.
  typedef ReferenceT       reference;
  /// The reference type of the const iterator.
  typedef const reference  const_reference;
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
  typedef const IteratorT const_iterator;

  /// The distance type of the iterator.
  typedef DifferenceT     difference_type;

  /// The iterator category of the iterator.
  typedef CategoryT       iterator_category;
};



} // namespace detail
} // namespace tupple
} // namespace boost

#endif
