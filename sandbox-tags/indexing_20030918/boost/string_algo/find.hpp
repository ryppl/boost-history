//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/compare.hpp>

namespace boost {

//  find_first  -----------------------------------------------//

    // find first
    /*
        Search for a first match of search sequence in the 
        input sequence. Result is given as a pair of iterators,
        delimiting the match.
    */
    template< typename InputT, typename SearchT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_first( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::first_finder(Search)(Input);
    }

    // find first ( case insensitive version )
    template< typename InputT, typename SearchT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    ifind_first( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::first_finder(Search,string_algo::is_iequal())(Input);
    }

//  find_last  -----------------------------------------------//

    // find last
    /*
        Search for a last match of search sequence in the 
        input sequence. Result is given as a pair of iterators,
        delimiting the match.
    */
    template< typename InputT, typename SearchT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_last( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::last_finder(Search)(Input);
    }

    // find last ( case insensitive version ) 
    template< typename InputT, typename SearchT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    ifind_last( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::last_finder(Search, string_algo::is_iequal())(Input);
    }

//  find_nth ----------------------------------------------------------------------//

    // find n-th
    /*
        Search for an n-th match of search sequence in the 
        input sequence. Result is given as a pair of iterators,
        delimiting the match.
    */
    template< typename InputT, typename SearchT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_nth( 
        InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::nth_finder(Search,Nth)(Input);
    }

    // find n-th ( case insensitive version )
    template< typename InputT, typename SearchT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    ifind_nth( 
        InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::nth_finder(Search,Nth,string_algo::is_iequal())(Input);
    }

//  find_head ----------------------------------------------------------------------//

    // find a head
    /*
        Get the head sequence of the input.
        Result is given as a pair of iterators, delimiting the match.
    */
    template< typename InputT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_head( 
        InputT& Input, 
        unsigned int N )
    {
        return string_algo::head_finder( N )( Input );      
    }

    // find a head
    /*
        Get the head sequence of the input.
        Result is given as a copy the head sequence.
    */
    template< typename InputT >
    inline InputT find_head_copy( 
        const InputT& Input, 
        unsigned int N )
    {
        return copy_range<InputT>( string_algo::head_finder(N)(Input) );
    }

//  find_tail ----------------------------------------------------------------------//

    // find a tail
    /*
        Get the tail sequence of the input.
        Result is given as a pair of iterators, delimiting the match.
    */
    template< typename InputT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_tail( 
        InputT& Input, 
        unsigned int N )
    {
        return string_algo::tail_finder( N )( Input );      
    }

    // find a tail
    /*
        Get the tail sequence of the input.
        Result is given as a copy the head sequence.
    */
    template< typename InputT >
    inline InputT find_tail_copy( 
        const InputT& Input, 
        unsigned int N )
    {
        return copy_range<InputT>( string_algo::tail_finder(N)(Input) );
    }

//  find_token --------------------------------------------------------------------//

    // find a token
    /*
        Get the token in the given sequence
        Result is given as a pair of iterators, delimiting the match.
    */
    template< typename InputT, typename PredicateT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_token( 
        InputT& Input,
        PredicateT Pred,
        bool bCompress=true )
    {
        return string_algo::token_finder( Pred, bCompress )( Input );       
    }


} // namespace boost


#endif  // BOOST_STRING_FIND_HPP
