
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_VIEW_TRAITS_DETAIL_HPP
#define BOOST_VIEW_TRAITS_DETAIL_HPP

#include <boost/config.hpp>
#include <boost/detail/iterator.hpp>

#include <map>

namespace boost {
  namespace view {
    namespace traits {

/**
 * @brief Generates types in much the same way as iterator_adaptor does.
 */
template< class IteratorT,
          class ConstIteratorT,
          class ValueT       = boost::detail::iterator_traits<IteratorT>::value_type,
          class ReferenceT   = boost::detail::iterator_traits<IteratorT>::reference,
          class PointerT     = boost::detail::iterator_traits<IteratorT>::pointer,
          class CategoryT    = boost::detail::iterator_traits<IteratorT>::iterator_category,
          class DifferenceT  = boost::detail::iterator_traits<IteratorT>::difference_type 
        >
struct default_iterator_traits {

  /// The value type of the iterator.
  typedef ValueT           value_type;

  /// The reference type of the iterator.
  typedef ReferenceT       reference;
  /// The reference type of the const iterator.
  typedef boost::detail::iterator_traits<ConstIteratorT>::reference const_reference;
  // Correct?
  // In connection with tuples:
  // const tuple<A&,B&> is NOT the same as tuple<const A&,const B&> !

  /// The pointer type of the iterator.
  typedef PointerT         pointer;
  /// The pointer type of the const iterator.
  typedef boost::detail::iterator_traits<ConstIteratorT>::pointer const_pointer;

  /// The iterator type.
  typedef IteratorT       iterator;
  /// The const iterator type.
  typedef ConstIteratorT  const_iterator;

  /// The distance type of the iterator.
  typedef DifferenceT     difference_type;

  /// The iterator category of the iterator.
  typedef CategoryT       iterator_category;
};


template< class AdaptedT,
          class ConstAdaptedT,
          class ValueT       = typename AdaptedT::value_type,
          class ReferenceT   = typename AdaptedT::reference,
          class PointerT     = typename AdaptedT::pointer,
          class CategoryT    = typename AdaptedT::iterator_category,
          class DifferenceT  = typename AdaptedT::difference_type 
        > 
struct adapted_iterator_traits
{
  /// The type of the elements accessed through the iterator.
  typedef ValueT      value_type;

  /// The type of the iterator.
  typedef AdaptedT                           iterator;
  /// The type of the const_iterator.
  typedef ConstAdaptedT                      const_iterator;

  /// The type of a reference to an element.
  typedef ReferenceT       reference;
  typedef typename ConstAdaptedT::reference  const_reference; // correct?
  /// The type of a pointer to an element.
  typedef PointerT         pointer;
  typedef typename ConstAdaptedT::pointer    const_pointer;


  typedef DifferenceT difference_type;

  typedef CategoryT iterator_category;

  // These are specific to iterator_adapted types, yet sometimes necessary:

  //typedef typename AdaptedT::base_type base_iterator;
  //typedef typename ConstAdaptedT::base_type const_base_iterator;

  //typedef typename AdaptedT::policies_type   policies_type;
  //typedef typename ConstAdaptedT::policies_type const_policies_type;

  // Not contained:

  // size_type

  // index_type
  // data_type
};



template<class SizeT,
         class IndexT,
         class DataT >
struct default_container_traits {

  typedef SizeT   size_type;

  typedef IndexT  index_type;
  typedef DataT   data_type;
};


template<class ContainerT> struct index_data_traits
{
  typedef typename ContainerT::size_type index_type;
  typedef typename ContainerT::reference data_type;
};


#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<class K,class V> struct index_data_traits< std::map<K,V> >
{
  typedef const std::map<K,V>::key_type index_type;
  typedef std::map<K,V>::data_type      data_type;
};

#endif


/**
 * Traits class for size_type, index_type, and data_type.
 */
template<class ContainerT,
         class IndexT      = index_data_traits<ContainerT>::index_type,
         class DataT       = index_data_traits<ContainerT>::data_type  >
struct adapted_container_traits {

  /// The size type of the container, and in particular, the result type of \c size().
  typedef typename ContainerT::size_type size_type;

  /// The index type of the container, i.e., the argument type of \c operator[].
  typedef IndexT           index_type;
  /// The data type of the container, i.e., the result type of \c operator[].
  typedef DataT            data_type;
};


    } // namespace ownership
  } // namespace view
} // namespace boost



#endif
