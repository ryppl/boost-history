//  Boost string_algo library replace2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE2_HPP
#define BOOST_STRING_REPLACE2_HPP

#include <deque>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/concept.hpp>
#include <boost/string_algo/detail/replace.hpp>
#include <boost/string_algo/detail/sequence.hpp>

/*! \file
	This header defines generic replace algorithms. Each algorithm replaces
	a part(s) of the input. The part to be replaced is found using a finder.
	Result of finding is then used by formatter to generate replacement.
*/

namespace boost {
    namespace string_algo {

// generic replace  -----------------------------------------------------------------//

        //! Generic replace algorithm
        /*!
            Create new sequence into output interator as a copy of input sequence,
            but with the match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

			\param Output A output iterarot to which the result will be copied
			\param Input An input sequence
			\param FindF A find functor used to search for a match to be replaced
			\param FormatF A format functor used to format a match
			\return An output iterator pointing just after last inserted character
        */
        template< 
            typename OutputIteratorT,
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline OutputIteratorT replace_copy(
            OutputIteratorT Output,
            const InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
			// Concept check
			function_requires< 
				FinderConcept<FindFT,
				BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();
			function_requires< 
				FormatterConcept<
					FormatFT,
					FindFT,BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();

            typedef detail::find_format_store<
                BOOST_STRING_TYPENAME 
                    container_traits<InputT>::const_iterator, FormatFT> store_type;

            // Create store for the find result
            store_type M( FormatF );

            // Find first match
            M=FindF( begin(Input), end(Input) );

            if ( M.empty() )
            {
                // Match not found - return original sequence
                std::copy( begin(Input), end(Input), Output );
                return Output;
            }

            // Copy the beginning of the sequence
            std::copy( begin(Input), begin(M), Output );
            // Format find result
            // Copy formated result
            std::copy( begin(M.format_result()), end(M.format_result()), Output );
            // Copy the rest of the sequence
            std::copy( M.end(), end(Input), Output );

            return Output;
        }

        //! Generic replace algorithm
        /*!
            Create new sequence as a copy the input sequence,
            but with the match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

			\param Input An input sequence
			\param FindF A find functor used to search for a match to be replaced
			\param FormatF A format functor used to format a match
			\return An output iterator pointing just after last inserted character
        */
        template< 
            typename InputT, 
            typename FindFT,
            typename FormatFT >
        inline InputT replace_copy(
            const InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
			// Concept check
			function_requires< 
				FinderConcept<FindFT,
				BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();
			function_requires< 
				FormatterConcept<
					FormatFT,
					FindFT,BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();

			typedef detail::find_format_store<
                BOOST_STRING_TYPENAME 
                    container_traits<InputT>::const_iterator, FormatFT> store_type;

            // Create store for the find result
            store_type M( FormatF );

            // Find first match
            M=FindF( begin(Input), end(Input) );

            if ( M.empty() )
            {
                // Match not found - return original sequence
                return InputT( Input );
            }

            InputT Output;
            // Copy the beginning of the sequence
            detail::insert( Output, end(Output), begin(Input), M.begin() );
            // Copy formated result
            detail::insert( Output, end(Output), M.format_result() );
            // Copy the rest of the sequence
            detail::insert( Output, end(Output), M.end(), end(Input) );

            return Output;
        }

        //! Generic replace algorithm
        /*!
            Replace a match in the input sequence with the formated substitute.
            The match is searched using find functor and substitute is formated using
            format functor.

			\param Input An input sequence
			\param FindF A find functor used to search for a match to be replaced
			\param FormatF A format functor used to format a match
			\return An output iterator pointing just after last inserted character
        */
        template<
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline InputT& replace( 
            InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
			// Concept check
			function_requires< 
				FinderConcept<FindFT,
				BOOST_STRING_TYPENAME container_traits<InputT>::iterator> >();
			function_requires< 
				FormatterConcept<
					FormatFT,
					FindFT,BOOST_STRING_TYPENAME container_traits<InputT>::iterator> >();

			typedef detail::find_format_store<
                BOOST_STRING_TYPENAME 
                    container_traits<InputT>::iterator, FormatFT> store_type;

            // Create store for the find result
            store_type M( FormatF );
            
            // Find range for the match
            M=FindF( begin(Input), end(Input) );

            if ( M.empty() )
            {
                // Search not found - return original sequence
                return Input;
            }

            // Replace match
            detail::replace( Input, M.begin(), M.end(), M.format_result() );
            
            return Input;
        }


//  replace_all generic ----------------------------------------------------------------//

        //! Generic replace all algorithm
        /*!
            Create new sequence into output interator as a copy of input sequence,
            but with the each match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

			\param Output A output iterarot to which the result will be copied
			\param Input An input sequence
			\param FindF A find functor used to search for a match to be replaced
			\param FormatF A format functor used to format a match
			\return An output iterator pointing just after last inserted character
        */
        template< 
            typename OutputIteratorT,
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline OutputIteratorT replace_all_copy(
            OutputIteratorT Output,
            const InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
			// Concept check
			function_requires< 
				FinderConcept<FindFT,
				BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();
			function_requires< 
				FormatterConcept<
					FormatFT,
					FindFT,BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();

			typedef BOOST_STRING_TYPENAME 
                container_traits<InputT>::const_iterator input_iterator_type; 
            typedef detail::find_format_store<
                input_iterator_type, FormatFT> store_type;
        
            // Create store for the find result
            store_type M( FormatF );

            input_iterator_type LastMatch=begin(Input);

            // Find first match
            M=FindF( begin(Input), end(Input) );

            // Iterate throug all matches
            while( !M.empty() )
            {
                // Copy the beginning of the sequence
                std::copy( LastMatch, M.begin(), Output );
                // Copy formated result
                std::copy( begin(M.format_result()), end(M.format_result()), Output );

                // Proceed to the next match
                LastMatch=M.end();
                M=FindF( LastMatch, end(Input) );
            }

            // Copy the rest of the sequence
            std::copy( LastMatch, end(Input), Output );

            return Output;
        }

        //! Generic replace all algorithm
        /*!
            Create new sequence as a copy the input sequence,
            but with the each match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

			\param Input An input sequence
			\param FindF A find functor used to search for a match to be replaced
			\param FormatF A format functor used to format a match
			\return An output iterator pointing just after last inserted character
        */
        template< 
            typename InputT, 
            typename FindFT,
            typename FormatFT >
        inline InputT replace_all_copy(
            const InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
			// Concept check
			function_requires< 
				FinderConcept<FindFT,
				BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();
			function_requires< 
				FormatterConcept<
					FormatFT,
					FindFT,BOOST_STRING_TYPENAME container_traits<InputT>::const_iterator> >();

			typedef BOOST_STRING_TYPENAME 
                container_traits<InputT>::const_iterator input_iterator_type; 
            typedef detail::find_format_store<
                input_iterator_type, FormatFT> store_type;
        
            // Create store for the find result
            store_type M( FormatF );

            // Initialize last match
            input_iterator_type LastMatch=begin(Input);
            // Find first match
            M=FindF( begin(Input), end(Input) );

            // Output temporary
            InputT Output;

            // Iterate throug all matches
            while( !M.empty() )
            {
                // Copy the beginning of the sequence
                detail::insert( Output, end(Output), LastMatch, M.begin() );
                // Copy formated result
                detail::insert( Output, end(Output), M.format_result() );

                // Proceed to the next match
                LastMatch=M.end();
                M=FindF( LastMatch, end(Input) );
            }

            // Copy the rest of the sequence
            detail::insert( Output, end(Output), LastMatch, end(Input) );

            return Output;
        }

        //! Generic replace all algorithm
        /*!
            Replace each match in the input sequence with the formated substitute.
            The match is searched using find functor and substitute is formated using
            format functor.

			\param Input An input sequence
			\param FindF A find functor used to search for a match to be replaced
			\param FormatF A format functor used to format a match
			\return An output iterator pointing just after last inserted character
        */
        template<
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline InputT& replace_all( 
            InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
			// Concept check
			function_requires< 
				FinderConcept<FindFT,
				BOOST_STRING_TYPENAME container_traits<InputT>::iterator> >();
			function_requires< 
				FormatterConcept<
					FormatFT,
					FindFT,BOOST_STRING_TYPENAME container_traits<InputT>::iterator> >();

			typedef BOOST_STRING_TYPENAME 
                container_traits<InputT>::iterator input_iterator_type; 
            typedef detail::find_format_store<
                input_iterator_type, FormatFT> store_type;
        
            // Create store for the find result
            store_type M( FormatF );
            
            // Instantiate replacement storage
            std::deque<BOOST_STRING_TYPENAME InputT::value_type> Storage;

            // Initialize replacement iterators
            input_iterator_type InsertIt=begin(Input);
            input_iterator_type SearchIt=begin(Input);
            
            // Find range for a first match
            M=FindF( begin(Input), end(Input) );

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
                detail::copy_to_storage( Storage, M.format_result() );

                // Find range for a next match
                M=FindF( SearchIt, end(Input) );
            }

            // process the last segment
            InsertIt=detail::process_segment( 
                Storage,
                Input,
                InsertIt,
                SearchIt,
                end(Input) );
            
            if ( Storage.empty() )
            {
                // Truncate input
                detail::erase( Input, InsertIt, end(Input) );
            }
            else
            {
                // Copy remaining data to the end of input
                detail::insert( Input, end(Input), Storage.begin(), Storage.end() );
            }

            return Input;
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_REPLACE2_HPP
