//  Boost string_algo library iterator_range.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ITERATOR_RANGE_HPP
#define BOOST_STRING_ITERATOR_RANGE_HPP

#include <boost/string_algo/config.hpp>
#include <utility>
#include <iterator>
#include <algorithm>
#include <boost/detail/iterator.hpp>

namespace boost {

//  iterator range template class -----------------------------------------//

    // iterator_range
    /*
        Allow to treat a pair of iterators as a sequence by 
        string algorithms.
    */
    template<typename IteratorT> 
    class iterator_range
    {
    public:
        typedef iterator_range<IteratorT> type;
        typedef BOOST_STRING_TYPENAME boost::detail::
            iterator_traits<IteratorT>::value_type value_type;
        typedef BOOST_STRING_TYPENAME boost::detail::
            iterator_traits<IteratorT>::reference reference;
        typedef BOOST_STRING_TYPENAME boost::detail::
            iterator_traits<IteratorT>::difference_type difference_type;
        typedef BOOST_STRING_TYPENAME boost::detail::
            iterator_traits<IteratorT>::difference_type size_type;
        typedef IteratorT const_iterator;
        typedef IteratorT iterator;

        // Constructors
        iterator_range() {}
        
        iterator_range( iterator Begin, iterator End ) : 
            m_Begin(Begin), m_End(End) {}

        iterator_range( const std::pair<IteratorT,IteratorT>& Range ) : 
            m_Begin(Range.first), m_End(Range.second) {}

        iterator_range( const iterator_range& Other ) :
            m_Begin(Other.begin()), m_End(Other.end()) {}

        template< typename OtherItT >
        iterator_range( const iterator_range<OtherItT>& Other ) :
            m_Begin(Other.begin()), m_End(Other.end()) {}

        // Assignment
        iterator_range& operator=( const iterator_range& Other )
        {
            m_Begin=Other.begin(); m_End=Other.end();
            return *this;
        }

        template< typename OtherItT >
        iterator_range& operator=( const iterator_range<OtherItT>& Other )
        {
            m_Begin=Other.begin(); m_End=Other.end();
            return *this;
        }

        // Comparison
        template< typename OtherItT > 
        bool operator==( const iterator_range<OtherItT>& Other ) const
        {
            return m_Begin==Other.begin() && m_End==Other.end();
        }

        template< typename OtherItT > 
        bool operator!=( const iterator_range<OtherItT>& Other ) const
        {
            return m_Begin!=Other.begin() || m_End!=Other.end();
        }

        // Operations
        IteratorT begin() const 
        { 
            return m_Begin; 
        }

        IteratorT end() const 
        { 
            return m_End; 
        } 

        bool empty() const 
        { 
            return m_Begin==m_End; 
        }
        
        difference_type size() const
        { 
            return std::distance( m_Begin, m_End ); 
        }

        void swap( iterator_range& Other )
        {
            std::swap( m_Begin, Other.begin() );
            std::swap( m_End, Other.end() );
        }

        // begin and end iterators
        IteratorT m_Begin;
        IteratorT m_End;
    };

//  iterator range utilities -----------------------------------------//

    // iterator_range contructor ( iterator version )
    template< typename IteratorT >
    inline iterator_range< IteratorT > make_range( IteratorT Begin, IteratorT End ) 
    {   
        return iterator_range<IteratorT>( Begin, End );
    }
  
    // iterator_range contructor ( iterator version )
    template< typename IteratorT >
    inline iterator_range< IteratorT > make_range( const std::pair<IteratorT,IteratorT>& Pair ) 
    {   
        return iterator_range<IteratorT>( Pair.first, Pair.second );
    }

    // copy a range into a sequence
    template< typename SeqT, typename IteratorT >
    inline SeqT copy_range( const iterator_range<IteratorT>& Range )
    {
        return SeqT( Range.begin(), Range.end() );
    }

    // transform a range into a sequence
    template< typename SeqT, typename IteratorT, typename FuncT >
    inline SeqT transform_range( const iterator_range<IteratorT>& Range, FuncT Func )
    {
        SeqT Seq;
        std::transform( Range.begin(), Range.end(), std::back_inserter(Seq), Func );
        return Seq;
    }

//  iterator range utilities (internal) ------------------------------//

    namespace string_algo {
        
        // copy range functor
        template< 
            typename SeqT, 
            typename IteratorT=BOOST_STRING_TYPENAME SeqT::const_iterator >
        struct copy_rangeF : 
            public std::unary_function< iterator_range<IteratorT>, SeqT >
        {
            SeqT operator()( const iterator_range<IteratorT>& Range ) const
            {
                return copy_range<SeqT>(Range);
            }
        };

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_ITERATOR_RANGE_HPP
