
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_TRAITS_DETAIL_HPP
#define BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_TRAITS_DETAIL_HPP

#include <boost/config.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>

namespace boost {
  namespace tupple {
    namespace detail {

// If one tries to compile something like
//   boost::detail::iterator_traits<int*> x
// MSVC returns with
//   error C2079: 'x' uses undefined struct 'boost::detail::must_manually_specialize_boost_detail_iterator_traits<Ptr>'
//
// Ok, lets do that (compare boost/iterator/iterator_traits.hpp, currently in the Sandbox):

template<class IteratorT> struct iterator_value
{
  typedef typename ::boost::mpl::if_<
            ::boost::is_pointer<IteratorT>,
              ::boost::remove_pointer<IteratorT>::type,
              ::boost::detail::iterator_traits<IteratorT>::value_type
          >::type type;
};


template<class IteratorT> struct iterator_reference
{
  typedef typename ::boost::mpl::if_<
            ::boost::is_pointer<IteratorT>,
              ::boost::add_reference< typename ::boost::remove_pointer<IteratorT>::type >::type,
              ::boost::detail::iterator_traits<IteratorT>::reference
          >::type type;
};


template<class IteratorT> struct iterator_pointer
{
  typedef typename ::boost::detail::iterator_traits<IteratorT>::pointer type;
};



/**
 * @brief Generates types in much the same way as iterator_adaptor does.
 */
template<class IteratorT,
         class ValueT       = iterator_value<IteratorT>::type,
         class ReferenceT   = iterator_reference<IteratorT>::type,
         class PointerT     = iterator_pointer<IteratorT>::type,
         class CategoryT    = ::boost::detail::iterator_traits<IteratorT>::iterator_category,
         class DifferenceT  = ::boost::detail::iterator_traits<IteratorT>::difference_type>
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
