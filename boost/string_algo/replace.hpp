//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include "traits.hpp"
#include "iterator_range.hpp"
#include "replace_impl.hpp"
#include "detail/find.hpp"
#include "detail/replace.hpp"

namespace boost {

//  replace_first --------------------------------------------------------------------//

    // replace_first iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_first_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
            string_algo::make_range( FormatBegin, FormatEnd ),
            Output,
            string_algo::detail::find_first_iterF< InputIteratorT, SearchIteratorT >(),
            string_algo::detail::identity_formatF< string_algo::iterator_range<FormatIteratorT> >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_first_copy(
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            Input,
            Search,
            Format,
            Output,
            string_algo::detail::find_first_constF<InputT, SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_first sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT replace_first_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input, Search, Format, 
            string_algo::detail::find_first_constF<InputT,SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_first in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT& replace_first( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, Search, Format, 
            string_algo::detail::find_firstF<InputT,SearchT>(),
            string_algo::detail::identity_formatF<FormatT>());
    }

//  replace_last --------------------------------------------------------------------//

    // replace_first iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_last_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            make_range( Begin, End ), 
            make_range( SearchBegin, SearchEnd ),
            make_range( FormatBegin, FormatEnd ),
            Output,
            string_algo::detail::find_last_iterF< InputIteratorT, SearchIteratorT >(),
            string_algo::detail::identity_formatF< string_algo::iterator_range<FormatIteratorT> >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_last_copy(
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            Input,
            Search,
            Format,
            Output,
            string_algo::detail::find_last_constF<InputT, SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_first sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT replace_last_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input, Search, Format, 
            string_algo::detail::find_last_constF<InputT,SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_first in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT& replace_last( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, Search, Format, 
            string_algo::detail::find_lastF<InputT,SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

//  replace_nth --------------------------------------------------------------------//

    // replace_nth iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_nth_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            make_range( Begin, End ), 
            make_range( SearchBegin, SearchEnd ),
            make_range( FormatBegin, FormatEnd ),
            Output,
            string_algo::detail::find_nth_iterF< InputIteratorT, SearchIteratorT >(Nth),
            string_algo::detail::identity_formatF< string_algo::iterator_range<FormatIteratorT> >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_nth_copy(
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            Input,
            Search,
            Format,
            Output,
            string_algo::detail::find_nth_constF<InputT, SearchT>(Nth),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_nth sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT replace_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input, Search, Format, 
            string_algo::detail::find_nth_constF<InputT,SearchT>(Nth),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_nth in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT& replace_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, Search, Format, 
            string_algo::detail::find_nthF<InputT,SearchT>(Nth),
            string_algo::detail::identity_formatF<FormatT>() );
    }


//  replace_all --------------------------------------------------------------------//
    
    // replace_all iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_all_copy(
            string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
            string_algo::make_range( FormatBegin, FormatEnd ),
            Output,
            string_algo::detail::find_first_iterF< InputIteratorT, SearchIteratorT >(),
            string_algo::detail::identity_formatF< string_algo::iterator_range<FormatIteratorT> >() );
    }

    // replace_all sequence version   
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_copy(
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format,
        OutputIteratorT Output )
    {
        return string_algo::replace_all_copy(
            Input,
            Search,
            Format,
            Output,
            string_algo::detail::find_first_constF<InputT, SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_all sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT replace_all_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy( 
            Input, Search, Format, 
            string_algo::detail::find_first_constF<InputT,SearchT>(),
            string_algo::detail::identity_formatF<FormatT>() );
    }

    // replace_all in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    InputT& replace_all( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all( 
            Input, Search, Format, 
            string_algo::detail::find_firstF<InputT,SearchT>(),
            string_algo::detail::identity_formatF<FormatT>());
    }
    

} // namespace boost


#endif  // BOOST_REPLACE_HPP
