
//  (C) Copyright Gennadiy Rozental 2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.


// cyclic_iterator is based upon the ideas of cycle_iterator by Gennadiy Rozental;
// an internal past_the_end counter was added; the equality operator was modified
// to overcome certain troubles. -RR


#ifndef CYCLIC_ITERATOR_HPP
#define CYCLIC_ITERATOR_HPP

#include<boost/iterator_adaptors.hpp>

#include<utility>
#include<algorithm>

namespace boost {
  namespace view {
    namespace detail {

template<typename BaseIterator>
class cyclic_iterator_policy : public default_iterator_policies {
    typedef boost::detail::iterator_traits<BaseIterator>::difference_type difference_type;
public:
    // Constructors
    cyclic_iterator_policy()
      : past_the_end( 0 ), rev( false )
    { }

    cyclic_iterator_policy( BaseIterator b, BaseIterator e, bool isRev = false )
      : m_bounds( b, e ), past_the_end( 0 ), rev( isRev )
    { }

private:
    template<class IterT> void to_the_right( IterT& x ) const
    {
      if( ++x.base() == x.policies().m_bounds.second )
      {
        x.base() = x.policies().m_bounds.first;
        ++x.policies().past_the_end;
      }
    }

    template<class IterT> void to_the_left( IterT& x ) const
    {
      if( x.base() == x.policies().m_bounds.first )
      {
        x.base() = x.policies().m_bounds.second;
        --x.policies().past_the_end;
      }
      --x.base();
    }

public:
    // policy implementation
    template<typename CyclicIterator>
    void    increment( CyclicIterator& x ) const
    {
      if( m_bounds.first == m_bounds.second ) { return; }

      if( x.policies().rev ) { to_the_left( x ); }
      else                   { to_the_right( x ); }
    }

    template<typename CyclicIterator>
    void    decrement( CyclicIterator& x ) const
    {
      if( m_bounds.first == m_bounds.second ) { return; }

      if( x.policies().rev ) { to_the_right( x ); }
      else                   { to_the_left( x ); }
    }

    template <typename CyclicIterator>
    void    advance( CyclicIterator& x, difference_type n ) const
    {
        difference_type cyclic_size 
          = x.policies().m_bounds.second - x.policies().m_bounds.first;

        if( cyclic_size == 0 ) { return; }

        // How many times will the iterator pass the end?
        int pe = ( x.policies().rev ? -n : n ) + (x.base() - m_bounds.first);

        int passed_end 
          = ( ( pe >= 0 ) ? ( pe / cyclic_size )
                          : ( ( (pe+1) / cyclic_size ) - 1 ) );
                                // -5 / 7 = 0; but we passed the end "-1" times
                                // -7 / 7 = -1; indeed, we passed "-1" times
                                // -8 / 7 = -1; should be "-2", and so on
        x.policies().past_the_end += passed_end;

        difference_type m = ( x.policies().rev ? -n : n );
        // Calculate modulus s.t. m is in {0,1,...,cycle_size-1}.
        if( m >= 0 ) { m = m % cyclic_size; }
        else         { m = cyclic_size - (-m % cyclic_size); }

        x.base() = m_bounds.first
                   + ( x.base() - m_bounds.first + m ) % cyclic_size;
    }


    template <class CyclicIterator1, class CyclicIterator2>
    difference_type
    distance(const CyclicIterator1& x, const CyclicIterator2& y) const
    {
        difference_type cyclic_size 
          = x.policies().m_bounds.second - x.policies().m_bounds.first;

        // What if x.policies().rev != y.policies().ref ?
        if( cyclic_size == 0 ) { return 0; }

        difference_type diff 
          = cyclic_size * ( y.policies().past_the_end - x.policies().past_the_end )
            + ( y.base() - x.base() );

        return ( x.policies().rev ? -diff : diff );
    }


    template <class CyclicIterator1, class CyclicIterator2>
    bool equal(const CyclicIterator1& x, const CyclicIterator2& y) const
    {
      return(    x.base() == y.base() && x.policies().rev == y.policies().rev
              && x.policies().past_the_end == y.policies().past_the_end );
    }

private:
    // Data members
    std::pair<BaseIterator,BaseIterator>    m_bounds;
    bool rev; // Reverse iterator

public: // public for testing
    int past_the_end; // Counts how often m_bounds.second was passed.
};

} // namespace detail


template<typename BaseIterator,
    class Value = boost::detail::iterator_traits<BaseIterator>::value_type,
    class Reference = Value&,
    class Pointer = Value*
>
struct cyclic_iterator_generator {
    typedef detail::cyclic_iterator_policy<BaseIterator>          policies_type;
    typedef boost::iterator_adaptor<BaseIterator,policies_type,
              Value,Reference,Pointer>           type;
};


template<typename BaseIterator, typename ConstBaseIterator,
         typename Value      = boost::detail::iterator_traits<BaseIterator>::value_type,
         typename ConstValue = boost::detail::iterator_traits<ConstBaseIterator>::value_type
>
struct cyclic_iterator_pair_generator
{
    typedef detail::cyclic_iterator_policy<BaseIterator>                         policies_type;
    typedef boost::iterator_adaptor<BaseIterator,policies_type,Value>           iterator;
    typedef boost::iterator_adaptor<ConstBaseIterator,policies_type,ConstValue> const_iterator;

};


template<typename BaseIterator>
typename cyclic_iterator_generator<BaseIterator>::type
make_cyclic_iterator( BaseIterator begin, BaseIterator end, bool isRev = false ) {
    typedef typename cyclic_iterator_generator<BaseIterator>::type ret_type;

    return ret_type( begin,
                     detail::cyclic_iterator_policy<BaseIterator>( begin, end, isRev ) );
}

template<class Container>
typename cyclic_iterator_generator<typename Container::iterator>::type
make_cyclic_iterator( Container& c, bool isRev = false  ) {
    typedef typename Container::iterator BaseIterator;
    typedef typename cyclic_iterator_generator<BaseIterator>::type ret_type;

    return ret_type( c.begin(),
                     detail::cyclic_iterator_policy<BaseIterator>( c.begin(), c.end(), isRev ) );
}


  } // namespace view
} // namespace boost

#endif

//  Revision History
//   29 Jan 02  Initial version (Gennadiy Rozental)
//    3 Feb 02  Eliminate raising of iterator category (Gennadiy Rozental;Thanks to Thomas Witt)
//    3 Feb 02  Added cycle_iterator_pair_generator (Gennadiy Rozental;Thanks to Thomas Witt for poining out interoperability issue)
//    4 Feb 02  Added distance implementation (Gennadiy Rozental;Thanks to Neal Backer for pointing out need for it)
