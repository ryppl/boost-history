//  Boost string_algo library string_erase_impl.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_IMPL_HPP
#define BOOST_STRING_ERASE_IMPL_HPP

#include "string_traits.hpp"

namespace boost {

	namespace string_algo {

// generic erase  -----------------------------------------------------------------//

		// erase
		/*
			Create new sequence into output interator as a copy of input sequence,
			but with match removed. The match is searched using find functor.
		*/
		template< 
			typename InputT, 
			typename SearchT,
			typename OutputIteratorT,
			typename FindFT >
		inline OutputIteratorT erase_copy(
			const InputT& Input,
			const SearchT& Search,
			OutputIteratorT Output,
			FindFT FindF )
		{
			typedef typename string_algo::
				search_traits<InputT, SearchT>::const_range_type input_range_type; 
			// Find first match
			input_range_type M=FindF( Input, Search );

			if ( M.empty() )
			{
				// Match not found - return original sequence
				std::copy( Input.begin(), Input.end(), Output );
				return Output;
			}

			// Copy the beginning of the sequence
			std::copy( Input.begin(), M.begin(), Output );
			// Copy the rest of the sequence
			std::copy( M.end(), Input.end(), Output );

			return Output;
		}

		// erase iterator version
		template< 
			typename InputIteratorT, 
			typename SearchIteratorT, 
			typename OutputIteratorT,
			typename FindFT >
		inline OutputIteratorT erase_copy(
			InputIteratorT Begin,
			InputIteratorT End,
			SearchIteratorT SearchBegin,
			SearchIteratorT SearchEnd,
			OutputIteratorT Output,
			FindFT FindF )
		{
			return erase_copy(
				string_algo::make_range( Begin, End ),
				string_algo::make_range( SearchBegin, SearchEnd ),
				Output,
				FindF );
		}

		// erase sequence version
		template< 
			typename InputT, 
			typename SearchT,
			typename FindFT >
		inline InputT erase_copy(
			const InputT& Input,
			const SearchT& Search,
			FindFT FindF )
		{
			typedef typename string_algo::
				search_traits<InputT, SearchT>::const_range_type input_range_type; 
			typedef typename string_algo::
				replace_format_traits<InputT, detail::identity_formatF<InputT> > replace_traits_type;

			// Instantiate insert functors
			replace_traits_type::insert_input_range_function_type InsInput;

			// Find first match
			input_range_type M=FindF( Input, Search );

			if ( M.empty() )
			{
				// Match not found - return original sequence
				return InputT( Input );
			}

			InputT Output;
			// Copy the beginning of the sequence
			InsInput( Output, Output.end(), Input.begin(), M.begin() );
			// Copy the rest of the sequence
			InsInput( Output, Output.end(), M.end(), Input.end() );

			return Output;
		}

		// erase in-place sequence version
		template<
			typename InputT,
			typename SearchT,
			typename FindFT >
		inline InputT& erase( 
			InputT& Input,
			const SearchT& Search,
			FindFT FindF )
		{
			typedef typename string_algo::
				search_traits<InputT, SearchT>::range_type input_range_type; 
			typedef typename string_algo::
				replace_format_traits<InputT, detail::identity_formatF<InputT> > replace_traits_type;

			// Instantiate replace functor
			replace_traits_type::erase_range_function_type Erase;

			// Find range for the match
			input_range_type M=FindF( Input, Search );

			if ( M.empty() )
			{
				// Search not found - return original sequence
				return Input;
			}

			// Replace match
			Erase( Input, M.begin(), M.end() );
	        
			return Input;
		}

//  erase_all generic ----------------------------------------------------------------//

		// erase_all generic version
		/*
			Create new sequence into output interator as a copy of input sequence,
			but with the all matches removed.
		*/
		template< 
			typename InputT, 
			typename SearchT,
			typename OutputIteratorT,
			typename FindFT >
		inline OutputIteratorT erase_all_copy(
			const InputT& Input,
			const SearchT& Search,
			OutputIteratorT Output,
			FindFT FindF )
		{
			typedef typename string_algo::
				search_traits<InputT, SearchT>::const_range_type input_range_type; 
			typedef typename string_algo::
				search_traits<InputT, SearchT>::input_const_iterator_type input_iterator_type; 
		
			input_iterator_type LastMatch=Input.begin();

			// Find first match
			input_range_type M=FindF( Input, Search );

			// Iterate throug all matches
			while( !M.empty() )
			{
				// Copy the part from the last match of the sequence
				std::copy( LastMatch, M.begin(), Output );

				// Proceed to the next match
				LastMatch=M.end();
				M=FindF( Input, Search, LastMatch );
			}

			// Copy the rest of the sequence
			std::copy( LastMatch, Input.end(), Output );

			return Output;
		}

		// erase_all generic version
		template< 
			typename InputIteratorT, 
			typename SearchIteratorT, 
			typename OutputIteratorT,
			typename FindFT >
		inline OutputIteratorT erase_all_copy(
			InputIteratorT Begin,
			InputIteratorT End,
			SearchIteratorT SearchBegin,
			SearchIteratorT SearchEnd,
			OutputIteratorT Output,
			FindFT FindF )
		{
			return erase_all_copy(
				string_algo::make_range( Begin, End ),
				string_algo::make_range( SearchBegin, SearchEnd ),
				Output,
				FindF );
		}

		// erase_all sequence version
		template< 
			typename InputT, 
			typename SearchT,
			typename FindFT >
		inline InputT erase_all_copy(
			const InputT& Input,
			const SearchT& Search,
			FindFT FindF )
		{
			typedef typename string_algo::
				search_traits<InputT, SearchT>::input_const_iterator_type input_iterator_type; 
			typedef typename string_algo::
				search_traits<InputT, SearchT>::const_range_type input_range_type; 
			typedef typename string_algo::
				replace_format_traits<InputT, detail::identity_formatF<InputT> > replace_traits_type;

			// Instantiate insert functors
			replace_traits_type::insert_input_range_function_type InsInput;

			// Initialize last match
			input_iterator_type LastMatch=Input.begin();

			// Find first match
			input_range_type M=FindF( Input, Search );

			InputT Output;

			// Iterate throug all matches
			while( !M.empty() )
			{
				// Copy the beginning of the sequence
				InsInput( Output, Output.end(), LastMatch, M.begin() );

				// Proceed to the next match
				LastMatch=M.end();
				M=FindF( Input, Search, LastMatch );
			}

			// Copy the rest of the sequence
			InsInput( Output, Output.end(), LastMatch, Input.end() );

			return Output;
		}

		// erase_all in-place sequence version
		template<
			typename InputT,
			typename SearchT,
			typename FindFT >
		inline InputT& erase_all( 
			InputT& Input,
			const SearchT& Search,
			FindFT FindF )
		{
			typedef typename string_algo::
				search_traits<InputT, SearchT>::input_iterator_type input_iterator_type; 
			typedef typename string_algo::
				search_traits<InputT, SearchT>::range_type input_range_type; 
			typedef typename string_algo::
				replace_format_traits<InputT, detail::identity_formatF<InputT> > replace_traits_type;

			// Instantiate replace functor
			replace_traits_type::erase_range_function_type Erase;

			// Find range for the match
			input_range_type M=FindF( Input, Search );

			while ( !M.empty() )
			{
				// Replace match
				input_iterator_type It=
					Erase( Input, M.begin(), M.end() );

				M=FindF( Input, Search, It );
			}

			return Input;
		}

	} // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_ERASE_IMPL_HPP
