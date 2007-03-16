
// Copyright Roland Richter 2001-2004.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_VIEW_FILTER_VIEW_HPP
#define BOOST_VIEW_FILTER_VIEW_HPP


#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>

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

  /// The type of the underlying container.
  typedef typename ownership::wrap<ContainerT>::domain domain_type;

  /// @name The iterator/container traits types.
  //@{    
  typedef typename 
    boost::filter_iterator<
      PredicateT,
      typename ownership::wrap<ContainerT>::domain::iterator
    > iterator;
    
  typedef typename boost::iterator_value<iterator>::type      value_type;
  typedef typename boost::iterator_reference<iterator>::type  reference;
  typedef typename boost::iterator_pointer<iterator>::type    pointer;
  typedef typename boost::iterator_difference<iterator>::type difference_type;
  
  typedef typename
    boost::filter_iterator<
      PredicateT,
      typename ownership::wrap<ContainerT>::domain::const_iterator
    > const_iterator;
  
  typedef typename boost::iterator_reference<const_iterator>::type  const_reference;
  typedef typename boost::iterator_pointer<const_iterator>::type    const_pointer;

  typedef typename domain_type::size_type size_type;
  //@}


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

  /// Returns a (copy of) the underlying container to allow write access.
  domain_type& domain()
    { return ownership::writeAccessTo( data ); }

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
  typename ownership::wrap<ContainerT>::type data;
  PredicateT p;
};


template< class ContainerT, class PredicateT >
inline filter_view<ContainerT, PredicateT> 
  make_filter_view( const ContainerT& theData, const PredicateT& theP )
{
  return filter_view<ContainerT, PredicateT>( theData, theP );
}


  } // namespace view
} // namespace boost


#endif
