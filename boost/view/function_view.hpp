
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_VIEW_FUNCTION_VIEW_HPP
#define BOOST_VIEW_FUNCTION_VIEW_HPP


#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "detail/traits_detail.hpp"

namespace boost {
  namespace view {

/**
 * A view which generates its elements "on the fly".
 *
 * In short, this is a combination of a transform view
 * of a range made up of counting iterators
 * (and that's exactly how it is implemented).
 *
 * Note that function view is somehow redundant.
 * A typical use of the view with, say, a function taking int's
 * as its arguments could be done with a transform iterator,
 * plus a counting iterator for the sake of efficiency.
 *
 * <h2>Template paramters</h2>
 * @param FunctionT The type of the generating function.
 * @param ArgT      The generating function's argument type.
 * @param ResT      The generating function's result type.
 */
#ifndef BOOST_MSVC
template< class FunctionT,
          class ArgT = typename FunctionT::argument_type,
          class ResT = typename FunctionT::result_type >
#else
template< class FunctionT,
          class ArgT = FunctionT::argument_type,
          class ResT = FunctionT::result_type >
#endif
class function_view
{
public:
  /// The view's own type.
  typedef function_view<FunctionT,ArgT,ResT> self_type;

  typedef traits::adapted_iterator_traits<
             boost::transform_iterator<
               FunctionT, boost::counting_iterator< ArgT > >,
             boost::transform_iterator<
               FunctionT, boost::counting_iterator< ArgT > >
           > iter_traits;

  typedef traits::default_container_traits<
             ArgT,
               // size_type is the return type of operator-( argument_type,argument_type ).
               // This "should" be convertible to argument_type; or should it be long int?
             ArgT,
             ResT
           > cont_traits;

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


  /// Creates a view of [theB,theE).
  function_view( index_type theB, index_type theE,
                 const FunctionT& theG = FunctionT() )
    : b( theB ), e( theE ), g( theG )
  { }

  /// The copy constructor.
  function_view( const function_view& rhs )
    : g( rhs.g ), b( rhs.b ), e( rhs.e )
  { }

  /// The destructor.
  ~function_view()
  { }

  /// Returns true iff the view's size is 0.
  bool      empty() const
  { return( size() <= 0); }

  /// Returns the size of the view.
  size_type size()  const
  //{ return std::distance( b, e ); }
  { return end() - begin(); }

  /// Returns a const_iterator pointing to the begin of the view.
  const_iterator begin() const { return const_iterator( b, g ); }

  /// Returns a const_iterator pointing to the end of the view.
  const_iterator end()   const { return const_iterator( e, g ); }

  /// Returns the value of the first element of the view.
  const_reference front() const { return *(begin()); }

  /// Returns the value of the last element in the view.
  const_reference back()  const { const_iterator tmp = end(); return *(--tmp); }

  /// @name Only supported if the functions' argument type is such that counting_iterator<ArgT> is random access.
  //@{
  /// This does *not* return the n'th element; instead, it returns the result
  /// of applying the generating function to n, i.e. it returns the value of g(n).
  /// Basically, for a function_view v which has g as the generating function,
  /// writing v[n] is the same as writing g(n).

  const data_type operator[]( index_type n ) const
  {
   typedef typename boost::detail::iterator_traits<
                      boost::counting_iterator< ArgT >
                    >::iterator_category category_type;
    return access( n, category_type() );
  }

  //@}

protected:
  // Depending on the iterator category, operator[] might select one of
  // two access functions:

  template<class CategoryTag> const data_type access( index_type n, CategoryTag dummy ) const
  { return g(n); }

#if 0 // Does not work for gcc-3.2 - is this even legal?
  template<> const data_type access< std::random_access_iterator_tag >( index_type n ) const
  { return begin()[boost::counting_iterator< ArgT >(n)-b]; }
#endif

protected:
  FunctionT g;
  boost::counting_iterator< ArgT > b,e;
};


  } // namespace view
} // namespace boost

#endif
