//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/string_algo/input_policy.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/find_impl.hpp>

namespace boost {

//  find_first  -----------------------------------------------//

    // find the first match a subsequnce in the sequence
    template< typename ForwardIterator1T, typename ForwardIterator2T >
    inline iterator_range<ForwardIterator1T>
    find_first( 
        ForwardIterator1T Begin, 
        ForwardIterator1T End, 
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::find( 
            Range,
            string_algo::detail::
                create_find_first( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ) ) );
    }

    // find_first sequence version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename string_algo::input_policy<InputT>::iterator_type >
    find_first( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::create_find_first( Input, Search ) );
    }

//  find_last  -----------------------------------------------//

    // find the last match a subsequnce in the sequence
    template< typename ForwardIterator1T, typename ForwardIterator2T >
    inline iterator_range<ForwardIterator1T>
    find_last( 
        ForwardIterator1T Begin, 
        ForwardIterator1T End, 
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::find( 
            Range,
            string_algo::detail::
                create_find_last( 
                        Range,
                        string_algo::make_range( SearchBegin, SearchEnd ) ) );
    }

    // find_last sequence version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename string_algo::input_policy<InputT>::iterator_type >
    find_last( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::create_find_last( Input, Search ) );
    }

//  find_nth ----------------------------------------------------------------------//

    // find the n-th match of a subsequnce in the sequence
    template< typename ForwardIterator1T, typename ForwardIterator2T >
    inline iterator_range<ForwardIterator1T>
    find_nth( 
        ForwardIterator1T Begin, 
        ForwardIterator1T End, 
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd,
        unsigned int Nth )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::find( 
            Range,
            string_algo::detail::
                create_find_nth( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ), Nth ) );
    }

    template< typename InputT, typename SearchT >
    inline iterator_range< typename string_algo::input_policy<InputT>::iterator_type >
    find_nth( 
        InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::create_find_nth( Input, Search, Nth ) );
    }

} // namespace boost


#endif  // BOOST_STRING_FIND_HPP
