//  Boost string_algo library iterator.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_DETAIL_ITERATOR_HPP
#define BOOST_STRING_DETAIL_ITERATOR_HPP

#include <iterator>
#include <boost/detail/iterator.hpp>

namespace boost {

//  iterator range  -----------------------------------------------//

    // iterator_range
    /*
        Allow to treat a pair of iterators as a sequence by 
        string algorithms.
    */
    template< typename IteratorT > class iterator_range
    {
    public:
        typedef iterator_range<IteratorT> type;
        typedef typename boost::detail::
            iterator_traits<IteratorT>::value_type value_type;
        typedef typename boost::detail::
            iterator_traits<IteratorT>::difference_type difference_type;
        typedef IteratorT const_iterator;
        typedef IteratorT iterator;

        // Constructors
        iterator_range() {}
        iterator_range( iterator Begin, iterator End ) : 
            m_Begin( Begin ), m_End( End ) {}

        template< typename OtherItT >
        iterator_range( const iterator_range<OtherItT>& Other ) :
            m_Begin( Other.begin() ), m_End( Other.end() ) {}

        // Assignment
        template< typename OtherItT >
        iterator_range& operator=( const iterator_range<OtherItT>& Other )
        {
            m_Begin=Other.begin(); m_End=Other.end();
            return *this;
        }

        // Comparison
        template< typename OtherItT > 
        bool operator==( const iterator_range<OtherItT>& Other )
        {
            return m_Begin==Other.begin() && m_End==Other.end();
        }

        template< typename OtherItT > 
        bool operator!=( const iterator_range<OtherItT>& Other )
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

    private:
        // begin and end iterators
        IteratorT m_Begin;
        IteratorT m_End;
    };

    namespace string_algo {

        // iterator_range contructor ( iterator version )
        template< typename IteratorT >
        inline iterator_range< IteratorT > make_range( IteratorT Begin, IteratorT End ) 
        {   
            return iterator_range< IteratorT >( Begin, End );
        }
        // iterator_range contructors ( sequence version )
        template< typename SeqT >
        inline iterator_range< typename SeqT::iterator > make_range( SeqT& Seq ) 
        {   
            return iterator_range< typename SeqT::iterator >( Seq.begin(), Seq.end() );
        }
        // iterator_range contructor ( const sequence version  )
        template< typename SeqT >
        inline iterator_range< typename SeqT::const_iterator > make_range_const( const SeqT& Seq ) 
        {   
            return iterator_range< typename SeqT::const_iterator >( Seq.begin(), Seq.end() );
        }

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_DETAIL_ITERATOR_HPP
