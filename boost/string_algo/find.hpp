//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include "config.hpp"
#include "traits.hpp"
#include "iterator_range.hpp"
#include "detail/find.hpp"
#include "find_impl.hpp"

namespace boost {

//  find_first  -----------------------------------------------//

    // find the first match a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline typename string_algo::
        iter_search_traits<InputIteratorT,SearchIteratorT>::range_type 
    find_first( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd )
    {
        // Forward the call to the functor
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::make_range( SearchBegin, SearchEnd ),
            string_algo::detail::find_first_iterF<InputIteratorT, SearchIteratorT>() );
    }

    // find_first sequence const version
    template< typename InputT, typename SearchT >
    inline typename string_algo::search_traits<InputT,SearchT>::const_range_type 
    find_first( 
        const InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            Search, 
            string_algo::detail::find_first_constF<InputT, SearchT>() );
    }

    // find_first sequence non-const version
    template< typename InputT, typename SearchT >
    inline typename string_algo::search_traits<InputT,SearchT>::range_type 
    BOOST_STRING_NON_CONST_FUNCTION(find_first)( 
        InputT& Input, 
        const SearchT& Search )
    {
        return BOOST_STRING_NON_CONST_FUNCTION(string_algo::find)( 
            Input, 
            Search, 
            string_algo::detail::find_firstF<InputT, SearchT>() );
    }

//  find_last  -----------------------------------------------//

    // find the last match a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline typename string_algo::
        iter_search_traits<InputIteratorT,SearchIteratorT>::range_type 
    find_last( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd )
    {
        // Forward the call to the functor
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::make_range( SearchBegin, SearchEnd ),
            string_algo::detail::find_last_iterF<InputIteratorT, SearchIteratorT>() );
    }

    // find_last sequence const version
    template< typename InputT, typename SearchT >
    inline typename string_algo::search_traits<InputT,SearchT>::const_range_type 
    find_last( 
        const InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            Search, 
            string_algo::detail::find_last_constF<InputT, SearchT>() );
    }

    // find_last sequence non-const version
    template< typename InputT, typename SearchT >
    inline typename string_algo::search_traits<InputT,SearchT>::range_type 
    BOOST_STRING_NON_CONST_FUNCTION(find_last)( 
        InputT& Input, 
        const SearchT& Search )
    {
        return BOOST_STRING_NON_CONST_FUNCTION(string_algo::find)( 
            Input, 
            Search, 
            string_algo::detail::find_lastF<InputT, SearchT>() );
    }

//  find_nth ----------------------------------------------------------------------//

    // find the n-th match of a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline typename string_algo::
        iter_search_traits<InputIteratorT,SearchIteratorT>::range_type 
    find_nth( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth )
    {
        // Forward the call to the functor
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::make_range( SearchBegin, SearchEnd ),
            string_algo::detail::find_nth_iterF<InputIteratorT, SearchIteratorT>(Nth) );
    }

    // find_nth sequence const version
    template< typename InputT, typename SearchT >
    inline typename string_algo::search_traits<InputT,SearchT>::const_range_type 
    find_nth( 
        const InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::find( 
            Input, 
            Search, 
            string_algo::detail::find_nth_constF<InputT, SearchT>(Nth) );
    }

    // find_nth sequence non-const version
    template< typename InputT, typename SearchT >
    inline typename string_algo::search_traits<InputT,SearchT>::range_type 
    BOOST_STRING_NON_CONST_FUNCTION(find_nth)( 
        InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return BOOST_STRING_NON_CONST_FUNCTION(string_algo::find)( 
            Input, 
            Search, 
            string_algo::detail::find_nthF<InputT, SearchT>(Nth) );
    }

} // namespace boost


#endif  // BOOST_STRING_FIND_HPP
