
// Copyright (C) 2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_STEP_ITERATOR_HPP
#define BOOST_STEP_ITERATOR_HPP

#include <boost/assert.hpp>
#include <boost/limits.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/next_prior.hpp>


namespace boost {

/**
 *
 * As a special case, a step_iterator with step=-1 is like a reverse_iterator.
 */

template< class Iterator
        , class ValueT        = use_default
        , class CategoryT     = use_default
        , class ReferenceT    = use_default
        , class DifferenceT   = use_default >
class step_iterator
  : public iterator_adaptor< 
             step_iterator<Iterator, ValueT, CategoryT, ReferenceT, DifferenceT>, 
             Iterator, ValueT, CategoryT, ReferenceT, DifferenceT >
{
  typedef iterator_adaptor< 
             step_iterator<Iterator, ValueT, CategoryT, ReferenceT, DifferenceT>, 
             Iterator, ValueT, CategoryT, ReferenceT, DifferenceT > super_type;

  friend class iterator_core_access;

public:
  typedef typename super_type::difference_type difference_type;

  static difference_type infinite_distance()
    { return std::numeric_limits<difference_type>::max(); }

  static difference_type negative_infinite_distance()
    { return std::numeric_limits<difference_type>::min(); }

  step_iterator()
    : step( 1 )
  {}

  explicit step_iterator( Iterator x, difference_type theStep = 1 )
    : super_type(x), step( theStep )
  {
    BOOST_ASSERT( step != 0 );
  }

  step_iterator( const step_iterator& rhs )
    : super_type( rhs.base() ), step( rhs.step )
  {}

  operator Iterator() const
    { return base(); }

private:

  typename super_type::reference dereference() const
  {
    if( step < 0 ) 
      return *boost::prior( base() );
    else
      return *base();
  }

  void increment()
    { base_reference() += step; }

  void decrement()
    { base_reference() -= step; }

  void advance( difference_type n )
  {
    base_reference() += (n * step); // Is this always defined?
  }

  template< class Other, class C, class D >
  difference_type distance_to( const step_iterator<Other, C, D >& rhs ) const
  {
    //### What if steps are different???
    difference_type d = rhs.base() - this->base();

    difference_type m = ( step > 0 ) ? ( d % step ) : ( d % -step );
    
    if( m == 0 )
      return d / step;
    else
    {
      bool positive = ( d > 0 && step > 0 ) || ( d < 0 && step < 0 );
      if( positive )
        return infinite_distance();
      else
        return negative_infinite_distance();
    }
  }

  template< class Other, class C, class D >
  bool equal( const step_iterator< Other, C, D >& rhs ) const
  {
    return( base() == rhs.base() && step == rhs.step );
  }

private: 
  difference_type step;
};


template< class Iterator, class Difference >
inline step_iterator<Iterator>
  make_step_iterator( Iterator x, Difference theStep = 1 )
{
  typedef step_iterator<Iterator> result_t;
  return result_t( x, theStep );
}

template< class Iterator >
inline bool is_reachable( step_iterator<Iterator> first, 
                          step_iterator<Iterator> last )
{
  step_iterator<Iterator>::difference_type d = last - first;
  return( 0 <= d && d < step_iterator<Iterator>::infinite_distance() );
}


} // namespace boost

#endif
