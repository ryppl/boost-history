//  Boost string_algo library string_erase.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_HPP
#define BOOST_STRING_ERASE_HPP

#include "string_traits.hpp"
#include "string_erase_impl.hpp"
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
        return string_algo::erase_copy(
			string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
            Output,
            string_algo::detail::find_first_iterF< InputIteratorT, SearchIteratorT >() );
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
        return string_algo::erase_copy(
            Input,
            Search,
            Output,
            string_algo::detail::find_first_constF<InputT, SearchT>() );
	}

    // erase_first sequence version
    template< typename InputT, typename SearchT >
    InputT erase_first_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::erase_copy( 
            Input, Search, 
			string_algo::detail::find_first_constF<InputT,SearchT>() );
    }

    // erase_first in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_first( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::erase( 
            Input, Search, 
            string_algo::detail::find_firstF<InputT,SearchT>() );
    }

//  erase_last --------------------------------------------------------------------//

    // erase_first iterator version
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
        return string_algo::erase_copy(
            make_range( Begin, End ), 
            make_range( SearchBegin, SearchEnd ),
            Output,
            string_algo::detail::find_last_iterF< InputIterator, SearchIterator >() );
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
        return string_algo::erase_copy(
            Input,
            Search,
            Output,
            string_algo::detail::find_last_constF<InputT, SearchT>() );
	}

	// erase_first sequence version
    template< typename InputT, typename SearchT >
    InputT erase_last_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::erase_copy( 
            Input, Search, 
            string_algo::detail::find_last_constF<InputT,SearchT>() );
    }

    // erase_first in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_last( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::erase( 
            Input, Search, Format, 
			string_algo::detail::find_lastF<InputT,SearchT>() );
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
        return string_algo::erase_copy(
            make_range( Begin, End ), 
            make_range( SearchBegin, SearchEnd ),
            Output,
            string_algo::detail::find_nth_iterF< InputIterator, SearchIterator >(Nth) );
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
        return string_algo::erase_copy(
            Input,
            Search,
            Output,
            string_algo::detail::find_nth_constF<InputT, SearchT>(Nth) );
	}

	// erase_nth sequence version
    template< typename InputT, typename SearchT >
    InputT erase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::erase_copy( 
            Input, Search, Format, 
            string_algo::detail::find_nth_constF<InputT,SearchT>(Nth) );
    }

    // erase_nth in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::erase( 
            Input, Search, 
            string_algo::detail::find_nthF<InputT,SearchT>(Nth) );
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
        return string_algo::erase_all_copy(
			string_algo::make_range( Begin, End ), 
            string_algo::make_range( SearchBegin, SearchEnd ),
            Output,
            string_algo::detail::find_first_iterF< InputIteratorT, SearchIteratorT >() );
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
        return string_algo::erase_all_copy(
            Input,
            Search,
            Output,
            string_algo::detail::find_first_constF<InputT, SearchT>() );
	}

    // erase_all sequence version
    template< typename InputT, typename SearchT >
    InputT erase_all_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::erase_all_copy( 
            Input, Search, 
			string_algo::detail::find_first_constF<InputT,SearchT>() );
    }

    // erase_all in-place sequence version
    template< typename InputT, typename SearchT >
    InputT& erase_all( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::erase_all( 
            Input, Search,  
            string_algo::detail::find_firstF<InputT,SearchT>() );
    }
	

} // namespace boost


#endif  // BOOST_ERASE_HPP
