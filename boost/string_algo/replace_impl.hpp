//  Boost string_algo library replace_impl.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_IMPL_HPP
#define BOOST_STRING_REPLACE_IMPL_HPP

#include <deque>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/replace.hpp>
#include <boost/string_algo/detail/container.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {
    namespace string_algo {

// generic formaters  ---------------------------------------------------------------//

		// identity formater
		template< typename ForwardIteratorT >
		inline detail::identity_formatF<
			iterator_range<ForwardIteratorT> >
		identity_formater( 
			ForwardIteratorT FormatBegin,
			ForwardIteratorT FormatEnd )
		{
			return detail::identity_formatF<
				iterator_range<ForwardIteratorT> >( make_range( FormatBegin, FormatEnd ) );
		}

		// identity formater
		template< typename ContainerT >
		inline detail::identity_formatF<ContainerT>
		identity_formater( const ContainerT& Format )
		{
			return detail::identity_formatF<ContainerT>( Format );
		}

		// empty formater
		template< typename IteratorT >
		inline detail::empty_formatF<
			BOOST_STRING_TYPENAME boost::detail::iterator_traits<IteratorT>::value_type >
		empty_formater( 
			IteratorT Begin,
			IteratorT End )
		{
			return detail::empty_formatF<
				BOOST_STRING_TYPENAME 
					boost::detail::iterator_traits<IteratorT>::value_type >();
		}

		template< typename ContainerT >
		inline detail::empty_formatF< 
			BOOST_STRING_TYPENAME container_traits<ContainerT>::value_type >
		empty_formater( const ContainerT& Input )
		{
			return detail::empty_formatF<
				BOOST_STRING_TYPENAME container_traits<ContainerT>::value_type >();
		}

// generic replace  -----------------------------------------------------------------//

        // replace 
        /*
            Create new sequence into output interator as a copy of input sequence,
            but with the match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor
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

        // replace sequence version
        template< 
            typename InputT, 
            typename FindFT,
            typename FormatFT >
        inline InputT replace_copy(
            const InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
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

        // replace in-place sequence version
        template<
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline InputT& replace( 
            InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
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

        // replace_all generic version
        /*
            Create new sequence into output interator as a copy of input sequence,
            but with the all matches replaced with substitute range.
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

        // replace_all sequence version
        template< 
            typename InputT, 
            typename FindFT,
            typename FormatFT >
        inline InputT replace_all_copy(
            const InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
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

        // replace_all in-place sequence version
        template<
            typename InputT,
            typename FindFT,
            typename FormatFT >
        inline InputT& replace_all( 
            InputT& Input,
            FindFT FindF,
            FormatFT FormatF )
        {
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


#endif  // BOOST_REPLACE_IMPL_HPP
