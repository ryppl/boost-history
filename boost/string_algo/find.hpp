//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/find_impl.hpp>

namespace boost {

//  find_first  -----------------------------------------------//

    // find the first match a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline iterator_range<InputIteratorT>
    find_first( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd )
    {
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_firstF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ) );
    }

    // find_first sequence const version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename InputT::const_iterator >
    find_first( 
        const InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::
                create_find_firstF<InputT>::create_const( Search ) );
    }

    // find_first sequence non-const version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename InputT::iterator >
    BOOST_STRING_MUTABLE_FUN(find_first)( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::
                create_find_firstF<InputT>::create( Search ) );
    }

//  find_last  -----------------------------------------------//

    // find the last match a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline iterator_range<InputIteratorT>
    find_last( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd )
    {
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_lastF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ) );
    }

    // find_last sequence const version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename InputT::const_iterator >
    find_last( 
        const InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::
                create_find_lastF<InputT>::create_const( Search ) );
    }

    // find_last sequence non-const version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename InputT::iterator >
    BOOST_STRING_MUTABLE_FUN(find_last)( 
        InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::
                create_find_lastF<InputT>::create( Search ) );
    }


//  find_nth ----------------------------------------------------------------------//

    // find the n-th match of a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline iterator_range<InputIteratorT>
    find_nth( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth )
    {
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_nthF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ), Nth ) );
    }

    // find_nth sequence const version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename InputT::const_iterator >
    find_nth( 
        const InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::
                create_find_nthF<InputT>::create_const( Search, Nth ) );
    }

    // find_nth sequence non-const version
    template< typename InputT, typename SearchT >
    inline iterator_range< typename InputT::iterator >
    BOOST_STRING_MUTABLE_FUN(find_nth)( 
        InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::
                create_find_nthF<InputT>::create( Search, Nth ) );
    }

} // namespace boost


#endif  // BOOST_STRING_FIND_HPP
