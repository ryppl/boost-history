
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_VIEW_FILTER_VIEW_HPP
#define BOOST_VIEW_FILTER_VIEW_HPP

#include <boost/config.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include "detail/traits_detail.hpp"
#include "detail/ownership_detail.hpp"

#include <iterator>

namespace boost {
  namespace view {

/**
 * A view which presents only those elements of the underlying container
 * for which a given predicate is true.
 *
 * <h2>Template paramters</h2>
 * @param ContainerT The type of the underlying container.
 * @param PredicateT The type of the filter predicate.
 */
template< class ContainerT, class PredicateT >
class filter_view
{
public:
  /// The view's own type.
  typedef filter_view<ContainerT,PredicateT> self_type;

  typedef traits::adapted_iterator_traits<
           boost::filter_iterator<
             PredicateT,
             typename ownership::wrap<ContainerT>::domain::iterator
#ifdef BOOST_MSVC
           , typename ownership::wrap<ContainerT>::domain::reference
#endif
           >,
           boost::filter_iterator<
             PredicateT,
             typename ownership::wrap<ContainerT>::domain::const_iterator
#ifdef BOOST_MSVC
           , typename ownership::wrap<ContainerT>::domain::const_reference
#endif
           >
         > iter_traits;

  typedef traits::adapted_container_traits< ownership::wrap<ContainerT>::domain >
           cont_traits;

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

  /// The type of the underlying container.
  typedef ownership::wrap<ContainerT>::domain domain_type;

  /// Creates a view of the given container and the given predicate.
  explicit filter_view( const domain_type& theData, const PredicateT& theP = PredicateT() )
    : data( theData ), p( theP )
  { }

  /// The copy constructor.
  filter_view( const filter_view& rhs )
    : data( rhs.data ), p( rhs.p )
  { }

  /// The destructor.
  ~filter_view()
  { }

  /// Returns true iff the view's size is 0.
  bool      empty() const
  {
    if( data->empty() ) return true;
    else                return( size() == 0 );
  }

  /// Returns the size of the view.
  size_type size()  const
  { return std::distance(begin(),end()); }

  /// Returns a const_iterator pointing to the begin of the view.
  const_iterator begin() const
  { return const_iterator( p, data->begin(), data->end() ); }
  /// Returns a iterator pointing to the begin of the view.
  iterator begin()
  { return iterator( p, data->begin(), data->end() ); }

  /// Returns a const_iterator pointing to the end of the view.
  const_iterator end() const
  { return const_iterator( p, data->end(), data->end() ); }
  /// Returns a iterator pointing to the end of the view.
  iterator end()
  { return iterator( p, data->end(), data->end() ); }

  /// Returns a const_reference for the first element of the view.
  const_reference front() const { return *(begin()); }
  /// Returns a reference for the first element of the view.
  reference front() { return *(begin()); }

  // back() and operator[] are not straight-forward,
  // since filter_iterator is forward only.

private:
  ownership::wrap<ContainerT>::type data;
  PredicateT p;
};


  } // namespace view
} // namespace boost


#endif
