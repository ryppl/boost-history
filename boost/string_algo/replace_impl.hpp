//  Boost string_algo library replace_impl.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_IMPL_HPP
#define BOOST_STRING_REPLACE_IMPL_HPP

#include <deque>
#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/replace.hpp>
#include <boost/string_algo/detail/container.hpp>

namespace boost {

    namespace string_algo {
 
// generic replace  -----------------------------------------------------------------//

        // replace 
        /*
            Create new sequence into output interator as a copy of input sequence,
            but with the match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor
        */
        template< 
            typename InputT, 
            typename OutputIteratorT,
            typename FindFT,
            typename FormatFT >
        inline OutputIteratorT replace_copy(
            OutputIteratorT Output,
            const InputT& Input,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            typedef typename FindFT::result_type search_result_type;
            
            // Find first match
            search_result_type M=FindF( Input );

            if ( M.empty() )
            {
                // Match not found - return original sequence
                std::copy( Input.begin(), Input.end(), Output );
                return Output;
            }

            // Copy the beginning of the sequence
            std::copy( Input.begin(), M.begin(), Output );
            // Format find result
            typename FormatFT::result_type FResult=FormatF( M );
            // Copy formated result
            std::copy( FResult.begin(), FResult.end(), Output );
            // Copy the rest of the sequence
            std::copy( M.end(), Input.end(), Output );

            return Output;
        }

        // replace iterator version
        template< 
            typename InputIteratorT, 
            typename OutputIteratorT,
            typename FindFT,
            typename FormatFT >
        inline OutputIteratorT replace_copy(
            OutputIteratorT Output,
            InputIteratorT Begin,
            InputIteratorT End,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            return replace_copy(
                make_range( Begin, End ),
                Output,
                FindF,
                FormatF );
        }

        // replace sequence version
        template< 
            typename InputT, 
            typename FindFT,
            typename FormatFT >
        inline InputT replace_copy(
            const InputT& Input,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            typedef typename FindFT::result_type search_result_type;

            // Find first match
            search_result_type M=FindF( Input );

            if ( M.empty() )
            {
                // Match not found - return original sequence
                return InputT( Input );
            }

            InputT Output;
            // Copy the beginning of the sequence
            detail::insert( Output, Output.end(), Input.begin(), M.begin() );
            // Copy formated result
            detail::insert( Output, Output.end(), FormatF( M ) );
            // Copy the rest of the sequence
            detail::insert( Output, Output.end(), M.end(), Input.end() );

            return Output;
        }

        // replace in-place sequence version
        template<
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline InputT& replace( 
            InputT& Input,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            typedef typename FindFT::result_type search_result_type;
            
            // Find range for the match
            search_result_type M=FindF( Input );

            if ( M.empty() )
            {
                // Search not found - return original sequence
                return Input;
            }

            // Replace match
            detail::replace( Input, M.begin(), M.end(), FormatF( M ) );
            
            return Input;
        }


//  replace_all generic ----------------------------------------------------------------//

        // replace_all generic version
        /*
            Create new sequence into output interator as a copy of input sequence,
            but with the all matches replaced with substitute range.
        */
        template< 
            typename InputT, 
            typename OutputIteratorT,
            typename FindFT,
            typename FormatFT >
        inline OutputIteratorT replace_all_copy(
            OutputIteratorT Output,
            const InputT& Input,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            typedef typename FindFT::result_type search_result_type;
            typedef typename InputT::const_iterator input_iterator_type; 
        
            input_iterator_type LastMatch=Input.begin();

            // Find first match
            search_result_type M=FindF( Input );

            // Iterate throug all matches
            while( !M.empty() )
            {
                // Copy the beginning of the sequence
                std::copy( LastMatch, M.begin(), Output );
                // Format find result
                typename FormatFT::result_type FResult=FormatF( M );
                // Copy formated result
                std::copy( FResult.begin(), FResult.end(), Output );

                // Proceed to the next match
                LastMatch=M.end();
                M=FindF( Input, LastMatch );
            }

            // Copy the rest of the sequence
            std::copy( LastMatch, Input.end(), Output );

            return Output;
        }

        // replace_all generic version
        template< 
            typename InputIteratorT, 
            typename OutputIteratorT,
            typename FindFT,
            typename FormatFT >
        inline OutputIteratorT replace_all_copy(
            OutputIteratorT Output,
            InputIteratorT Begin,
            InputIteratorT End,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            return replace_all_copy(
                make_range( Begin, End ),
                Output,
                FindF,
                FormatF );
        }

        // replace_all sequence version
        template< 
            typename InputT, 
            typename FindFT,
            typename FormatFT >
        inline InputT replace_all_copy(
            const InputT& Input,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            typedef typename FindFT::result_type search_result_type;
            typedef typename InputT::const_iterator input_iterator_type; 

            // Initialize last match
            input_iterator_type LastMatch=Input.begin();
            // Find first match
            search_result_type M=FindF( Input );

            // Output temporary
            InputT Output;

            // Iterate throug all matches
            while( !M.empty() )
            {
                // Copy the beginning of the sequence
                detail::insert( Output, Output.end(), LastMatch, M.begin() );
                // Copy formated result
                detail::insert( Output, Output.end(), FormatF( M ) );

                // Proceed to the next match
                LastMatch=M.end();
                M=FindF( Input, LastMatch );
            }

            // Copy the rest of the sequence
            detail::insert( Output, Output.end(), LastMatch, Input.end() );

            return Output;
        }

        // replace_all in-place sequence version
        template<
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline InputT& replace_all( 
            InputT& Input,
            const FindFT& FindF,
            const FormatFT& FormatF )
        {
            typedef typename FindFT::result_type search_result_type;
            typedef typename InputT::iterator input_iterator_type; 
            
            // Instantiate replacement storage
            std::deque<typename InputT::value_type> Storage;

            // Initialize replacement iterators
            input_iterator_type InsertIt=Input.begin();
            input_iterator_type SearchIt=Input.begin();
            
            // Find range for a first match
            search_result_type M=FindF( Input );

            while ( !M.empty() )
            {
                // process the segment
                InsertIt=detail::process_segment( 
                    Storage,
                    Input,
                    InsertIt,
                    SearchIt,
                    M.begin() );
                
                // Adjust search iterator
                SearchIt=M.end();

                // Copy formated replace to the storage
                detail::copy_to_storage( Storage, FormatF(M) );

                // Find range for a next match
                M=FindF( Input, SearchIt );
            }

            // process the last segment
            InsertIt=detail::process_segment( 
                Storage,
                Input,
                InsertIt,
                SearchIt,
                Input.end() );
            
            if ( Storage.empty() )
            {
                // Truncate input
                detail::erase( Input, InsertIt, Input.end() );
            }
            else
            {
                // Copy remaining data to the end of input
                detail::insert( Input, Input.end(), Storage.begin(), Storage.end() );
            }

            return Input;
        }

    } // namespace string_algo
    
} // namespace boost


#endif  // BOOST_REPLACE_IMPL_HPP
