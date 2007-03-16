
// Copyright Roland Richter 2001-2004.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_VIEW_RANGE_VIEW_HPP
#define BOOST_VIEW_RANGE_VIEW_HPP

#include <boost/detail/iterator.hpp>


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
  typedef range_view<IteratorT,ConstIteratorT,ValueT,ReferenceT,PointerT,CategoryT,DifferenceT> self_type;

  /// @name The traits types visible to the public.
  //@{           
  typedef ValueT       value_type;
  
  typedef IteratorT         iterator;
  typedef ReferenceT        reference;
  typedef PointerT          pointer;

  typedef ConstIteratorT    const_iterator;
  typedef boost::detail::iterator_traits<ConstIteratorT>::reference  const_reference;
  typedef boost::detail::iterator_traits<ConstIteratorT>::pointer    const_pointer;
  
  typedef DifferenceT  difference_type;
  
  typedef std::size_t       size_type;
  typedef std::size_t       index_type;
  typedef ReferenceT        data_type;
  
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
