
// Copyright (C) 2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

// cyclic_iterator is based upon the ideas of cycle_iterator by Gennadiy Rozental.
// This version is a complete rewrite to work with new iterator_adaptor's. -RR


#ifndef BOOST_CYCLIC_ITERATOR_HPP
#define BOOST_CYCLIC_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>

#include <iterator>
#include <utility>


namespace boost {

template< class BaseIterator
        , class ValueT        = use_default
        , class CategoryT     = use_default
        , class ReferenceT    = use_default
        , class DifferenceT   = use_default >
class cyclic_iterator
  : public iterator_adaptor< 
             cyclic_iterator<BaseIterator, ValueT, CategoryT, ReferenceT, DifferenceT>, 
             BaseIterator, ValueT, CategoryT, ReferenceT, DifferenceT >
{
  typedef iterator_adaptor< 
             cyclic_iterator<BaseIterator, ValueT, CategoryT, ReferenceT, DifferenceT>, 
             BaseIterator, ValueT, CategoryT, ReferenceT, DifferenceT > super_t;

  friend class iterator_core_access;

public:

    typedef typename super_t::difference_type difference_type;

    cyclic_iterator()
      : pastTheEnd( 0 ), isReverse( false )
    { }

    cyclic_iterator( BaseIterator b, BaseIterator e, bool isRev = false )
      : super_t( b ),
        bounds( b, e ), pastTheEnd( 0 ), isReverse( isRev )
    { }

private:

    void to_the_right()
    {
      if( ++base_reference() == bounds.second )
      {
        base_reference() = bounds.first;
        ++pastTheEnd;
      }
    }

    void to_the_left()
    {
      if( base_reference() == bounds.first )
      {
        base_reference() = bounds.second;
        --pastTheEnd;
      }
      --base_reference();
    }


    void increment()
    {
      if( bounds.first == bounds.second ) { return; }

      if( this->isReverse ) { to_the_left(); }
      else                  { to_the_right(); }
    }

    void decrement()
    {
      if( bounds.first == bounds.second ) { return; }

      if( this->isReverse ) { to_the_right(); }
      else                  { to_the_left(); }
    }


    void advance( difference_type n )
    {
      difference_type cycle_size 
        = std::distance( bounds.first, bounds.second );

      if( cycle_size == 0 ) { return; }

      // How many times will the iterator pass the end?

      difference_type m = ( this->isReverse ? -n : n );
      int pe = m + ( base() - bounds.first );
      int passedEnd 
        = ( ( pe >= 0 )                          // -5 / 7 =  0; but we passed the end "-1" times
            ? ( pe / cycle_size )                // -7 / 7 = -1; indeed, we passed "-1" times
            : ( ( (pe+1) / cycle_size ) - 1 ) ); // -8 / 7 = -1; should be "-2", and so on
                              
      this->pastTheEnd += passedEnd;

      // Calculate modulus s.t. m is in {0,1,...,cycle_size-1}.
      if( m >= 0 ) { m = m % cycle_size; }
      else         { m = cycle_size - (-m % cycle_size); }

      base_reference() = bounds.first
                           + ( base() - bounds.first + m ) % cycle_size;
    }


    template< class OtherBase, class V, class C, class R, class D >
    difference_type distance_to( const cyclic_iterator< OtherBase, V, C, R, D >& y ) const
    {
      difference_type cycle_size 
        = std::distance( bounds.first, bounds.second );

      //### What if isReverse != y.isReverse ?
      if( cycle_size == 0 ) { return 0; }

      difference_type diff 
        = cycle_size * ( y.pastTheEnd - this->pastTheEnd )
            + std::distance( this->base(), y.base() );

      return ( this->isReverse ? -diff : diff );
    }


    template< class OtherBase, class V, class C, class R, class D >
    bool equal( const cyclic_iterator< OtherBase, V, C, R, D >& y ) const
    {
      return(    this->base() == y.base() && this->isReverse == y.isReverse
              && this->pastTheEnd == pastTheEnd );
    }

private:

    std::pair <BaseIterator, BaseIterator > bounds;
    bool                                    isReverse;

public: // public for testing
    int pastTheEnd; // Counts how often bounds.second was passed.
};



template< class BaseIterator >
  cyclic_iterator<BaseIterator>
  make_cyclic_iterator( BaseIterator begin, BaseIterator end, bool isRev = false ) 
{
  return cyclic_iterator<BaseIterator>( begin, end, isRev );
}

template< class Container >
  cyclic_iterator<typename Container::iterator>
  make_cyclic_iterator( Container& c, bool isRev = false  ) 
{
  typedef typename Container::iterator BaseIterator;
  return cyclic_iterator<BaseIterator>( c.begin(), c.end(), isRev );
}



} // namespace boost

#endif
