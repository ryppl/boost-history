
// Copyright (C) 2001,2002 Roland Richter (roland@flll.jku.at)
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef RANGE_VIEW_HPP
#define RANGE_VIEW_HPP

#include <boost/iterator_adaptors.hpp>

#include "detail/traits_detail.hpp"


namespace boost {
  namespace view {

/**
 * A view which adds a container-like interface to a pair of iterators.
 *
 * <h2>Template paramters</h2>
 * @param IteratorT The type of the underlying iterator.
 * @param ConstIteratorT The corresponding const iterator.
 *
 */
template< class IteratorT,
          class ConstIteratorT,
          class ValueT       = boost::detail::iterator_traits<IteratorT>::value_type,
          class ReferenceT   = boost::detail::iterator_traits<IteratorT>::reference,
          class PointerT     = boost::detail::iterator_traits<IteratorT>::pointer,
          class CategoryT    = boost::detail::iterator_traits<IteratorT>::iterator_category,
          class DifferenceT  = boost::detail::iterator_traits<IteratorT>::difference_type 
        >
class range_view
{
public:
  /// The view's own type.
  typedef range_view<IteratorT,ConstIteratorT> self_type;

  typedef traits::default_iterator_traits< 
    IteratorT, ConstIteratorT,
    ValueT, ReferenceT, PointerT, CategoryT, DifferenceT > iter_traits;

  typedef traits::default_container_traits< long,long, ReferenceT > cont_traits;

  /// @name The traits types visible to the public.
  //@{           
  typedef typename iter_traits::value_type       value_type;
  
  typedef typename iter_traits::iterator         iterator;
  typedef typename iter_traits::const_iterator   const_iterator;
  typedef typename iter_traits::reference        reference;
  typedef typename iter_traits::const_reference  const_reference;
  typedef typename iter_traits::pointer          pointer;
  typedef typename iter_traits::const_pointer    const_pointer;
  
  typedef typename iter_traits::difference_type  difference_type;
  
  typedef typename cont_traits::size_type        size_type;
  typedef typename cont_traits::index_type       index_type;
  typedef typename cont_traits::data_type        data_type;
  
  //@}

  /// Creates a view of range [theB,theE).
  range_view( iterator theB, iterator theE )
    : b( theB ), e( theE )
  { }

  /// The copy constructor.
  range_view( const range_view& rhs )
    : b( rhs.b ), e( rhs.e )
  { }

  /// The destructor.
  ~range_view()
  { }

  /// Returns true iff the view's size is 0.
  bool      empty() const
  { return( begin() == end() ); }

  /// Returns the size of the view.
  size_type size()  const
  { return std::distance(begin(),end()); }

  /// Returns a const_iterator pointing to the begin of the view.
  const_iterator begin() const { return b; }
  /// Returns an iterator pointing to the begin of the view.
  iterator begin() { return b; }

  /// Returns a const_iterator pointing to the end of the view.
  const_iterator end() const { return e; }
  /// Returns an iterator pointing to the end of the view.
  iterator end() { return e; }

  /// Returns a const_reference for the first element of the view.
  const_reference front() const { return *(begin()); }
  /// Returns a reference for the first element of the view.
  reference       front()       { return *(begin()); }
  /// Returns a const_reference for the last element in the view.
  const_reference back()  const { const_iterator tmp = end(); return *(--tmp); }
  /// Returns a reference for the last element in the view.
  reference       back()        { iterator tmp = end();       return *(--tmp); }

  /// Returns the n'th element of the view.
  const data_type operator[](index_type n) const
  { return *(begin() + n); }
  /// Returns the n'th element of the view.
        data_type operator[](index_type n)
  { return *(begin() + n); }

private:
  iterator b;
  iterator e;
};


  } // namespace view
} // namespace boost


#endif
