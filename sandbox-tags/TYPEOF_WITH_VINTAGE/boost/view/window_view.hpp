
// Copyright Roland Richter 2001-2004.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_VIEW_WINDOW_VIEW_HPP
#define BOOST_VIEW_WINDOW_VIEW_HPP

#include <boost/config.hpp>
#include "cyclic_iterator.hpp"
#include <boost/iterator/iterator_traits.hpp>

#include "detail/ownership_detail.hpp"


namespace boost {
  namespace view {

/**
 * A view which provides access to a window (that is, a subrange) of the container.
 *
 * <h2>Template paramters</h2>
 * @param ContainerT The type of the underlying container.
 *
 */
template<class ContainerT>
class window_view
{
public:
  /// The view's own type.
  typedef window_view<ContainerT> self_type;

  /// The type of the underlying container.
  typedef ownership::wrap<ContainerT>::domain domain_type;

  /// The type of the underlying containers' const iterator.
  typedef typename domain_type::const_iterator domain_const_iterator;

  /// @name The traits types visible to the public.
  //@{
  typedef boost::cyclic_iterator<
              typename ownership::wrap<ContainerT>::domain::iterator
#ifdef BOOST_MSVC
            , boost::use_default
            , boost::use_default
            , typename ownership::wrap<ContainerT>::domain::reference
#endif
                                > iterator;
    
  typedef typename boost::iterator_value<iterator>::type      value_type;
  typedef typename boost::iterator_reference<iterator>::type  reference;
  typedef typename boost::iterator_pointer<iterator>::type    pointer;
  typedef typename boost::iterator_difference<iterator>::type difference_type;

  typedef boost::cyclic_iterator<
              typename ownership::wrap<ContainerT>::domain::const_iterator
#ifdef BOOST_MSVC
            , boost::use_default
            , boost::use_default
            , typename ownership::wrap<ContainerT>::domain::const_reference
#endif
                                > const_iterator;
  
  typedef typename boost::iterator_reference<const_iterator>::type  const_reference;
  typedef typename boost::iterator_pointer<const_iterator>::type    const_pointer;


  typedef typename domain_type::size_type size_type;

  typedef typename domain_type::size_type       index_type;
  typedef typename boost::iterator_reference<iterator>::type       data_type;
  //@}

  /// Creates a view of range [theB,theE).
  window_view( const domain_type& theData, domain_const_iterator theB, domain_const_iterator theE )
    : data( theData )
  { select( theB-theData.begin(), std::distance( theB, theE ) ); }

  window_view( const domain_type& theData, domain_const_iterator theB, difference_type theSize )
    : data( theData )
  { select( theB-theData.begin(), theSize ); }

  window_view( const domain_type& theData, size_type theI, difference_type theSize )
    : data( theData )
  { select( theI, theSize ); }

  /// The copy constructor.
  window_view( const window_view& rhs )
    : data( rhs.data ), b( rhs.b ), e( rhs.e )
  { }

  /// The destructor.
  ~window_view()
  { }

  /// Returns true iff the view's size is 0.
  bool      empty() const
  {
    if( data->empty() ) return true;
    else return( size() == 0 );
  }

  /// Returns the size of the view.
  size_type size()  const
  {
    return std::distance(b,e);
  }

  /// Returns a \c const_iterator pointing to the begin of the view.
  const_iterator begin() const { return b; }
  /// Returns an \c iterator pointing to the begin of the view.
  iterator begin() { return b; }

  /// Returns a \c const_iterator pointing to the end of the view.
  const_iterator end() const { return e; }
  /// Returns an \c iterator pointing to the end of the view.
  iterator end() { return e; }

  /// @name Only supported if container is a sequence:
  //@{
  /// Returns a \c const_reference for the first element of the view, i.e., the first element of the window.
  const_reference front() const { return *(begin()); }
  /// Returns a \c reference for the first element of the view, i.e., the first element of the window.
  reference       front()       { return *(begin()); }
  /// Returns a \c const_reference for the last element in the view, i.e., the last element of the window.
  const_reference back()  const { const_iterator tmp = e; return *(--tmp); }
  /// Returns a \c reference for the last element in the view, i.e., the last element of the window.
  reference       back()        { iterator tmp = e;       return *(--tmp); }
  //@}

  /// @name Only supported if container is random access:
  //@{
  /// Returns the n'th element of the view, i.e., the n'th element of the window.
  const data_type operator[](index_type n) const { return *(this->begin() + n); }
  /// Returns the n'th element of the view, i.e., the n'th element of the window.
        data_type operator[](index_type n)       { return *(this->begin() + n); }

  /// Sets the window to be the range [theI,theI+theSize).
  // NOTE: Since we refer to data->begin() and data->end() here, we can't accept pointers
  //          which (possibly) point to a copy of data.
  void select( index_type theI, difference_type theSize )
  {
    if( theSize >= 0 )
    {
      b = iterator( data->begin(), data->end() ) + theI;
      e = b + theSize;
    }
    else // theSize < 0
    {
      b = iterator( data->begin(), data->end(), true ) - theI;
      e = b - theSize;
    }

  }
  //@}

  /// Shifts the window n steps to right if n>0 and left if n<0.
  void shift( difference_type n = 1 )
  {
    std::advance(b,n);
    std::advance(e,n);
  }

private:
  ownership::wrap<ContainerT>::type data;

  iterator b;
  iterator e;
};

  } // namespace view
} // namespace boost

#endif
