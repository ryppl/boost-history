
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_VIEW_TRANSFORM_VIEW_HPP
#define BOOST_VIEW_TRANSFORM_VIEW_HPP

#include <boost/iterator/transform_iterator.hpp>

#include "detail/traits_detail.hpp"
#include "detail/ownership_detail.hpp"

namespace boost {
  namespace view {

/**
 * @brief A view which returns the elements of the underlying collection,
 *        transformed by a function.
 *
 * <h2>Template paramters</h2>
 * @param ContainerT The type of the underlying container.
 * @param FunctionT The type of the transformation.
 *
 */
template< class ContainerT, class FunctionT >
class transform_view
{
private:
  typedef traits::adapted_iterator_traits<
             boost::transform_iterator<
               FunctionT, typename ownership::wrap<ContainerT>::domain::iterator >,
             boost::transform_iterator<
               FunctionT, typename ownership::wrap<ContainerT>::domain::const_iterator >
           > iter_traits;

  typedef traits::adapted_container_traits<
             ownership::wrap<ContainerT>::domain,
             typename traits::index_data_traits< typename ownership::wrap<ContainerT>::domain >::index_type,
             typename FunctionT::result_type
           > cont_traits;

public:
  /// The view's own type (i.e. transform_view<...>).
  typedef transform_view< ContainerT, FunctionT > self_type;

  /// The type of the underlying container.
  typedef ownership::wrap<ContainerT>::domain domain_type;

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

  /// Creates a view of container \c theData transformed by the function \c theF.
  explicit transform_view( const domain_type& theData, const FunctionT& theF = FunctionT() )
    : data( theData ), f( theF )
  { }

  /// The copy constructor.
  transform_view( const transform_view& rhs )
    : data( rhs.data ), f( rhs.f )
  { }

  /// The destructor.
  ~transform_view()
  { }

  /// Returns true iff the view's size is 0.
  bool      empty() const { return data->empty(); }

  /// Returns the size of the view.
  /** In this case, the size of the view is always the same as the size of the underlying container.
   */
  size_type size()  const { return data->size(); }

  /// Swaps the contents of two views.
  self_type& swap( self_type& rhs )
  {
    std::swap( data, rhs.data );
    std::swap( f, rhs.f );
    return *this;
  }

  /// Returns a \c const_iterator pointing to the begin of the view.
  const_iterator begin() const { return const_iterator( data->begin(), f ); }

  /// Returns a \c const_iterator pointing to the end of the view.
  const_iterator end()   const { return const_iterator( data->end(), f ); }

  /// @name Only supported if container is a sequence:
  //@{
  /// Returns the value of the first element of the view.
  value_type front() const { return f( data->front() ); }
  //@}

  /// @name Only supported if container is a back insertion sequence:
  //@{
  /// Returns the value of the last element of the view.
  value_type back()  const { return f( data->back() ); }
  //@}


  /// @name Only supported if container is random access:
  //@{
  /// Returns the n'th element of the view.
  const data_type operator[](index_type n) const { return f( (*data)[n] ); }
  //@}

private:
  ownership::wrap<ContainerT>::type data;
  FunctionT                         f;
};


  } // namespace view
} // namespace boost

#endif

