//  Boost string_algo library iterator.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ITERATOR_RANGE_HPP
#define BOOST_STRING_ITERATOR_RANGE_HPP

#include <utility>
#include <iterator>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/detail/iterator_range_base.hpp>

namespace boost {

//  iterator range  -----------------------------------------------//

    // iterator_range
    /*
        Allow to treat a pair of iterators as a sequence by 
        string algorithms.
    */
    template<typename IteratorT> 
    class iterator_range : 
        public string_algo::detail::iterator_range_base<IteratorT>
    {
    public:
        typedef iterator_range<IteratorT> type;
        typedef string_algo::detail::iterator_range_base<IteratorT> base_type;
        typedef typename base_type::value_type value_type;
        typedef typename base_type::reference reference;
        typedef typename base_type::difference_type difference_type;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::iterator iterator;

        // Constructors
        iterator_range() : base_type() {}
        
        iterator_range( iterator Begin, iterator End ) : 
            base_type( Begin, End ) {}

        iterator_range( const std::pair<IteratorT,IteratorT>& Range ) : 
            base_type( Range.first, Range.second ) {}

        iterator_range( const iterator_range& Other ) :
            base_type( Other.begin(), Other.end() ) {}

        template< typename OtherItT >
        iterator_range( const string_algo::detail::iterator_range_base<OtherItT>& Other ) :
            base_type( Other.begin(), Other.end() ) {}

        // Assignment
        iterator_range& operator=( const iterator_range& Other )
        {
            base_type::operator=( Other );
            return *this;
        }

        template< typename OtherItT >
        iterator_range& operator=( const string_algo::detail::iterator_range_base<OtherItT>& Other )
        {
            base_type::operator=( Other );
            return *this;
        }
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


#endif  // BOOST_STRING_ITERATOR_RANGE_HPP
