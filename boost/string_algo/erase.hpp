//  Boost string_algo library erase.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_HPP
#define BOOST_STRING_ERASE_HPP

#include "traits.hpp"
#include "iterator_range.hpp"
#include "replace_impl.hpp"
#include "detail/find.hpp"
#include "detail/replace.hpp"

namespace boost {

//  erase_first  --------------------------------------------------------//

    // erase_first iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_first_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
			0,
            Output,
            string_algo::detail::find_first_iterF< InputIteratorT, SearchIteratorT >(),
			string_algo::detail::null_formatF<string_algo::iterator_range<InputIteratorT> > () );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_first_copy(
        const InputT& Input,
        const SearchT& Search,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            Input,
            Search,
			0,
            Output,
            string_algo::detail::find_first_constF<InputT, SearchT>(),
			string_algo::detail::null_formatF<InputT>() );
    }

    // erase_first sequence version
    template< typename InputT, typename SearchT >
    InputT erase_first_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
			Search, 
			0,
            string_algo::detail::find_first_constF<InputT,SearchT>(),
			string_algo::detail::null_formatF<InputT>());
    }

    // erase_first in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_first( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
			Search,
			0,
            string_algo::detail::find_firstF<InputT,SearchT>(),
			string_algo::detail::null_formatF<InputT>()	);
    }

//  erase_last  --------------------------------------------------------//

    // erase_last iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_last_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
			0,
            Output,
            string_algo::detail::find_last_iterF< InputIteratorT, SearchIteratorT >(),
			string_algo::detail::null_formatF<string_algo::iterator_range<InputIteratorT> >());
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_last_copy(
        const InputT& Input,
        const SearchT& Search,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            Input,
            Search,
			0,
            Output,
            string_algo::detail::find_last_constF<InputT, SearchT>(),
			string_algo::detail::null_formatF<InputT>() );
    }

    // erase_last sequence version
    template< typename InputT, typename SearchT >
    InputT erase_last_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
			Search, 
			0,
            string_algo::detail::find_last_constF<InputT,SearchT>(),
			string_algo::detail::null_formatF<InputT>());
    }

    // erase_last in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_last( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
			Search,
			0,
            string_algo::detail::find_lastF<InputT,SearchT>(),
			string_algo::detail::null_formatF<InputT>()	);
    }

//  erase_nth --------------------------------------------------------------------//

    // erase_nth iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_nth_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            make_range( Begin, End ), 
            make_range( SearchBegin, SearchEnd ),
			0,
            Output,
            string_algo::detail::find_nth_iterF< InputIteratorT, SearchIteratorT >(Nth),
			string_algo::detail::null_formatF<string_algo::iterator_range<InputIteratorT> >());
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_nth_copy(
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        OutputIteratorT Output )
    {
        return string_algo::replace_copy(
            Input,
            Search,
			0,
            Output,
            string_algo::detail::find_nth_constF<InputT, SearchT>(Nth),
			string_algo::detail::null_formatF<InputT>() );
    }

    // erase_nth sequence version
    template< typename InputT, typename SearchT >
    InputT erase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy( 
            Input, Search, Format, 0, 
            string_algo::detail::find_nth_constF<InputT,SearchT>(Nth),
			string_algo::detail::null_formatF<InputT>() );
    }

    // erase_nth in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace( 
            Input, Search, 0,
            string_algo::detail::find_nthF<InputT,SearchT>(Nth),
			string_algo::detail::null_formatF<InputT>() );
    }


//  erase_all --------------------------------------------------------------------//
    
    // erase_all iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_all_copy(
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        OutputIteratorT Output )
    {
        return string_algo::replace_all_copy(
            string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
			0,
            Output,
            string_algo::detail::find_first_iterF< InputIteratorT, SearchIteratorT >(),
			string_algo::detail::null_formatF<string_algo::iterator_range<InputIteratorT> >());
    }

    // erase_all sequence version   
    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_all_copy(
        const InputT& Input,
        const SearchT& Search,
        OutputIteratorT Output )
    {
        return string_algo::replace_all_copy(
            Input,
            Search,
			0,
            Output,
            string_algo::detail::find_first_constF<InputT, SearchT>(),
			string_algo::detail::null_formatF<InputT>() );
    }

    // erase_all sequence version
    template< typename InputT, typename SearchT >
    InputT erase_all_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy( 
            Input, Search, 0,
            string_algo::detail::find_first_constF<InputT,SearchT>(),
			string_algo::detail::null_formatF<InputT>());
    }

    // erase_all in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_all( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all( 
            Input, Search, 0,
            string_algo::detail::find_firstF<InputT,SearchT>(),
			string_algo::detail::null_formatF<InputT>() );
    }
    

} // namespace boost


#endif  // BOOST_ERASE_HPP
