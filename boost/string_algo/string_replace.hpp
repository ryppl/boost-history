//  Boost string_algo library string_replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include <utility>

#include "string_funct.hpp"
#include "string_traits.hpp"
#include "string_find.hpp"

namespace boost {

//  replace_substr ----------------------------------------------------------------------//

	namespace string_algo {
		
		namespace detail {

			// Replace a range in the sequence with another range
			/*
			Returns the iterator pointing just after inserted subrange.
			Note:
			Function first tries to replace elements in the input sequence,
			up to To position with the elements form Repl sequence.
			The rest of the Repl is then inserted thereafter.
			If the Repl sequence is shorter, overlaping elements are erased from the Input
			*/
			template< typename InputSeqT, typename ReplSeqT >
				inline typename InputSeqT::iterator
				replace_substr(
				InputSeqT& Input,
				typename InputSeqT::iterator From,
				typename InputSeqT::iterator To,
				ReplSeqT& Repl )
			{       
				typename InputSeqT::iterator InsertIt=From;
				bool bReplace=(InsertIt!=To);

				for( typename ReplSeqT::const_iterator ReplIt=Repl.begin();
					ReplIt!=Repl.end();
					ReplIt++)
				{
					if ( bReplace )
					{
						// Replace mode
						*InsertIt=*ReplIt;

						InsertIt++;
						bReplace=(InsertIt!=To);
					}
					else
					{
						// Insert mode
						InsertIt=Input.insert( InsertIt, *ReplIt );
						// Advance to the next item
						InsertIt++;
					}
				}

				// Erase the overlapping elements
				if ( bReplace )
				{
					InsertIt=Input.erase( InsertIt, To );
				}

				// Return InputIt iterator
				return InsertIt;
			}
		
		} // namespace detail
	
	} // namespace string_algo

// generic replace  -----------------------------------------------------------------//

    // replace iterator version
    /*
        Create new sequence into output interator as a copy of input sequence,
        but with the match replaced with substitute range. The match is searched 
		using find functor.
    */
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator,
		typename FindT >
    inline OutputIterator replace_copy(
        InputIterator Begin, InputIterator End,
        MatchIterator MatchBegin, MatchIterator MatchEnd,
        ReplIterator ReplBegin, ReplIterator ReplEnd,
        OutputIterator Output,
		FindT Find )
    {
        // Find first match
        typename string_algo::search_traits<InputIterator, MatchIterator>::range_type M=
            Find( Begin, End, MatchBegin, MatchEnd );

        if ( M.first==M.second )
        {
            // Match not found - return original sequence
            std::copy( Begin, End, Output );
            return Output;
        }

        // Copy the beggining of the sequence
        std::copy( Begin, M.first, Output );
        // Copy repl range
        std::copy( ReplBegin, ReplEnd, Output );
        // Copy the rest of the sequence
        std::copy( M.second, End, Output );

		return Output;
    }

    // replace_first sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT,
        typename ReplSeqT,
		typename FindT >
    InputSeqT replace_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl,
		FindT Find )
    {
        InputSeqT Output;
        replace_copy( 
            Input.begin(), Input.end(), 
            Match.begin(), Match.end(), 
            Repl.begin(), Repl.end(), 
            std::back_inserter( Output ),
			Find );

        return Output;
    }

    // replace in-place sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT,
        typename ReplSeqT,
		typename FindF >
    InputSeqT& replace( 
        InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl,
		FindF Find )
    {
        // Find range for the match
        typename string_algo::search_traits<
			typename InputSeqT::iterator, 
			typename MatchSeqT::const_iterator>::range_type M=
				Find( Input.begin(), Input.end(), Match.begin(), Match.end() );

        if ( M.first==M.second )
        {
            // Match not found - return original sequence
            return Input;
        }

        // Replace match
		string_algo::detail::replace_substr( Input, M.first, M.second, Repl );
        
        return Input;
    }

//  replace_first --------------------------------------------------------------------//

    // replace_first iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator >
    inline OutputIterator replace_first_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output )
    {
		return replace_copy(
			Begin, End, 
			MatchBegin, MatchEnd,
			ReplBegin, ReplEnd,
			Output,
			string_algo::find_firstF< InputIterator, MatchIterator >() );
	}

    // replace_first sequence version
    template< typename InputSeqT, typename MatchSeqT, typename ReplSeqT >
    InputSeqT replace_first_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl )
    {
        return replace_copy( 
			Input, Match, Repl, 
			string_algo::find_firstF< 
				typename InputSeqT::const_iterator,
				typename MatchSeqT::const_iterator >() );
    }

    // replace_first in-place sequence version
    template< typename InputSeqT, typename MatchSeqT, typename ReplSeqT >
    InputSeqT& replace_first( 
        InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl )
    {
        return replace( 
			Input, Match, Repl, 
			string_algo::find_firstF< 
				typename InputSeqT::iterator,
				typename MatchSeqT::const_iterator >() );
    }

//  replace_last --------------------------------------------------------------------//

    // replace_last iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator >
    inline OutputIterator replace_last_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output )
    {
		return replace_copy(
			Begin, End, 
			MatchBegin, MatchEnd,
			ReplBegin, ReplEnd,
			Output,
			string_algo::find_lastF< InputIterator, MatchIterator >() );
	}

    // replace_last sequence version
    template< typename InputSeqT, typename MatchSeqT, typename ReplSeqT >
    InputSeqT replace_last_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl )
    {
        return replace_copy( 
			Input, Match, Repl, 
			string_algo::find_lastF< 
				typename InputSeqT::const_iterator,
				typename MatchSeqT::const_iterator >() );
    }

    // replace_last in-place sequence version
    template< typename InputSeqT, typename MatchSeqT, typename ReplSeqT >
    InputSeqT& replace_last( 
        InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl )
    {
        return replace( 
			Input, Match, Repl, 
			string_algo::find_lastF< 
				typename InputSeqT::iterator,
				typename MatchSeqT::const_iterator >() );
    }

//  replace_nth --------------------------------------------------------------------//

    // replace_nth iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator >
    inline OutputIterator replace_nth_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
		unsigned int Nth,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output )
    {
		return replace_copy(
			Begin, End, 
			MatchBegin, MatchEnd,
			ReplBegin, ReplEnd,
			Output,
			string_algo::find_nthF< InputIterator, MatchIterator >(Nth) );
	}

    // replace_nth sequence version
    template< typename InputSeqT, typename MatchSeqT, typename ReplSeqT >
    InputSeqT replace_nth_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
		unsigned int Nth,
        const ReplSeqT& Repl )
    {
        return replace_copy( 
			Input, Match, Repl, 
			string_algo::find_nthF< 
				typename InputSeqT::const_iterator,
				typename MatchSeqT::const_iterator >(Nth) );
    }

    // replace_nth in-place sequence version
    template< typename InputSeqT, typename MatchSeqT, typename ReplSeqT >
    InputSeqT& replace_nth( 
        InputSeqT& Input,
        const MatchSeqT& Match,
		unsigned int Nth,
        const ReplSeqT& Repl )
    {
        return replace( 
			Input, Match, Repl, 
			string_algo::find_nthF< 
				typename InputSeqT::iterator,
				typename MatchSeqT::const_iterator >(Nth) );
    }

//  replace_all    ----------------------------------------------------------------//

    // replace_all iterator version
    /*
        Create new sequence into output interator as a copy of input sequence,
        but with the all matches replaced with substitute range
    */
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator >
    inline void replace_all_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output )
    {
        // Find the first match
        InputIterator LastMatch=Begin;
        typename string_algo::search_traits<InputIterator, MatchIterator>::range_type M=
            find_first( Begin, End, MatchBegin, MatchEnd );

        // Iterate throug all matches
        while( M.first != M.second )
        {
            // Copy the beggining of the sequence
            std::copy( LastMatch, M.first, Output );
            // Copy repl range
            std::copy( ReplBegin, ReplEnd, Output );

            // Proceed to the next match
            LastMatch=M.second;
            M=find_first( LastMatch, End, MatchBegin, MatchEnd );
        }

        // Copy the rest of the sequence
        std::copy( LastMatch, End, Output );
    }

    // replace_all sequence version
    template<
        typename InputSeqT,
        typename MatchSeqT,
        typename ReplSeqT >
    InputSeqT replace_all_copy( 
        const InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl )
    {
        InputSeqT output;
        replace_all_copy( 
            Input.begin(), Input.end(), 
            Match.begin(), Match.end(), 
            Repl.begin(), Repl.end(), 
            std::back_inserter( output ) );

        return output;
    }

    // replace_all in-place sequence version
    /*
        Makes in-place replacement of all matches in the seqence
    */
    template<
        typename InputSeqT,
        typename MatchSeqT,
        typename ReplSeqT >
    InputSeqT& replace_all( 
        InputSeqT& Input,
        const MatchSeqT& Match,
        const ReplSeqT& Repl )
    {
        // Find the first match
        typename string_algo::search_traits<
			typename InputSeqT::iterator, 
			typename MatchSeqT::iterator>::range_type M=
            find_first( Input.begin(), Input.end(), Match.begin(), Match.end() );

        // Iterate throug all matches
        while( M.first != M.second )
        {
            // Replace the match
            typename InputSeqT::iterator It=string_algo::detail::replace_substr(
                Input,
                M.first, M.second,
                Repl );

            // Proceed to the next match
            M=find_first( It, Input.end(), Match.begin(), Match.end() );
        }
            
        return Input;
    }

} // namespace boost


#endif  // BOOST_REPLACE_HPP
