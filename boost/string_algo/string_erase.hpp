//  Boost string_algo library string_erase.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_HPP
#define BOOST_STRING_ERASE_HPP

#include <utility>

#include "string_find.hpp"
#include "string_replace.hpp"

namespace boost {


// generic erase  -----------------------------------------------------------------//
/*  
    erase is implemented by calling replace_* variant with empty repl sequence 
*/

    // erase iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator,
		typename FindT >
    inline OutputIterator erase_copy(
        InputIterator Begin, InputIterator End,
        MatchIterator MatchBegin, MatchIterator MatchEnd,
        OutputIterator Output,
		FindT Find )
    {
        return replace_copy( Begin, End, MatchBegin, MatchBegin, End, End, Output, Find );
    }

    // erase sequence version
    template< typename InputSeqT, typename MatchSeqT, typename FindT >
    InputSeqT erase_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
		FindT Find )
    {
        return replace_copy( Input, Match, InputSeqT(), Find );
    }

    // replace in-place sequence version
    template< typename InputSeqT, typename MatchSeqT, typename FindF >
    InputSeqT& erase( 
        InputSeqT& Input,
        const MatchSeqT& Match,
		FindF Find )
    {
        return replace( Input, Match, InputSeqT(), Find );
    }


//  erase_first  --------------------------------------------------------//

    // erase_first iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator >
    inline OutputIterator erase_first_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output )
    {
		return erase_copy(
			Begin, End, 
			MatchBegin, MatchEnd,
			Output,
			string_algo::find_firstF< InputIterator, MatchIterator >() );
    }

    // erase_first sequence version
    template< typename InputSeqT, typename MatchSeqT >
    InputSeqT erase_first_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match )
    {
        return erase_copy( 
			Input, Match, 
			string_algo::find_firstF< 
				typename InputSeqT::const_iterator,
				typename MatchSeqT::const_iterator >() );
    }

    // erase_first in-place sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT >
    InputSeqT& erase_first( 
        InputSeqT& Input,
        const MatchSeqT& Match )
    {
        return erase( 
			Input, Match, 
			string_algo::find_firstF< 
				typename InputSeqT::iterator,
				typename MatchSeqT::const_iterator >() );
    }

//  erase_last  --------------------------------------------------------//

    // erase_last iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator >
    inline OutputIterator erase_last_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output )
    {
		return erase_copy(
			Begin, End, 
			MatchBegin, MatchEnd,
			Output,
			string_algo::find_lastF< InputIterator, MatchIterator >() );
    }

    // erase_last sequence version
    template< typename InputSeqT, typename MatchSeqT >
    InputSeqT erase_last_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match )
    {
        return erase_copy( 
			Input, Match, 
			string_algo::find_lastF< 
				typename InputSeqT::const_iterator,
				typename MatchSeqT::const_iterator >() );
    }

    // erase_last in-place sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT >
    InputSeqT& erase_last( 
        InputSeqT& Input,
        const MatchSeqT& Match )
    {
        return erase( 
			Input, Match, 
			string_algo::find_lastF< 
				typename InputSeqT::iterator,
				typename MatchSeqT::const_iterator >() );
    }

//  erase_nth  --------------------------------------------------------//

    // erase_nth iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator >
    inline OutputIterator erase_nth_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
		unsigned int Nth,
        OutputIterator Output )
    {
		return erase_copy(
			Begin, End, 
			MatchBegin, MatchEnd,
			Output,
			string_algo::find_nthF< InputIterator, MatchIterator >(Nth) );
    }

    // erase_nth sequence version
    template< typename InputSeqT, typename MatchSeqT >
    InputSeqT erase_nth_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
		unsigned int Nth ) 
    {
        return erase_copy( 
			Input, Match, 
			string_algo::find_nthF< 
				typename InputSeqT::const_iterator,
				typename MatchSeqT::const_iterator >(Nth) );
    }

    // erase_nth in-place sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT >
    InputSeqT& erase_nth( 
        InputSeqT& Input,
        const MatchSeqT& Match, 
		unsigned int Nth )
    {
        return erase( 
			Input, Match, 
			string_algo::find_nthF< 
				typename InputSeqT::iterator,
				typename MatchSeqT::const_iterator >(Nth) );
    }


//  erase_all  --------------------------------------------------------//

    // erase_all iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator >
    inline void erase_all_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output )
    {
        replace_all_copy( Begin, End, MatchBegin, MatchEnd, End, End, Output );
    }

    // erase_all sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT >
    InputSeqT erase_all_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match )
    {
        return replace_all_copy( Input, Match, InputSeqT() );
    }

    // erase_all in-place sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT >
    InputSeqT& erase_all( 
        InputSeqT& Input,
        const MatchSeqT& Match )
    {
        return replace_all( Input, Match, InputSeqT() );
    }

} // namespace boost


#endif  // BOOST_ERASE_HPP
