//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {

//  replace_range --------------------------------------------------------------------//

    // replace_range iterator version
    template< 
        typename OutputIteratorT,
        typename ForwardIterator1T, 
        typename ForwardIterator2T >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator1T SearchBegin,
        ForwardIterator1T SearchEnd,
        ForwardIterator2T FormatBegin,
        ForwardIterator2T FormatEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );
            
        return string_algo::replace_copy(
            Output,
            Range,
            string_algo::detail::
                create_find_range( Range, string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<ForwardIterator2T> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_range output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename FormatT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_range(
                    Input,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_range sequence version
    template< typename InputT, typename FormatT >
    inline InputT replace_range_copy( 
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_range(
                    Input,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_range in-place sequence version
    template< typename InputT, typename FormatT >
    inline InputT& replace_range( 
        InputT& Input,
        typename InputT::iterator SearchBegin,
        typename InputT::iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_range(
                    Input,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_first --------------------------------------------------------------------//

    // replace_first iterator version
    template< 
        typename OutputIteratorT,
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename ForwardIterator3T >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd,
        ForwardIterator3T FormatBegin,
        ForwardIterator3T FormatEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );
        
        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_first( 
                        Range,      
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<ForwardIterator3T> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_first output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_first sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_first_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_first in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_first( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_last --------------------------------------------------------------------//

    // replace_last iterator version
    template< 
        typename OutputIteratorT,
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename ForwardIterator3T >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd,
        ForwardIterator3T FormatBegin,
        ForwardIterator3T FormatEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_lastF( 
                        Range,
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<ForwardIterator3T> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_last output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_last( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_last sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_last_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_last( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_last in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_last( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_last( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_nth --------------------------------------------------------------------//

    // replace_nth iterator version
    template< 
        typename OutputIteratorT,
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename ForwardIterator3T >
    inline OutputIteratorT replace_nth_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd,
        unsigned int Nth,
        ForwardIterator3T FormatBegin,
        ForwardIterator3T FormatEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_nth( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ), Nth ),
            string_algo::detail::
                identity_formatF< iterator_range<ForwardIterator3T> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_nth( Input, Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_nth sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_nth( Input, Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_nth in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_nth( Input, Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }


//  replace_all --------------------------------------------------------------------//

    // replace_all iterator version
    template< 
        typename OutputIteratorT,
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename ForwardIterator3T >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd,
        ForwardIterator3T FormatBegin,
        ForwardIterator3T FormatEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_all_copy(
            Output,
            Range,
            string_algo::detail::
                create_find_first( 
                        Range,
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<ForwardIterator3T> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_all iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_all sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_all_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_all in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_all( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }
    

} // namespace boost


#endif  // BOOST_REPLACE_HPP
