//  Boost string_algo library replace_all.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_ALL_DETAIL_HPP
#define BOOST_STRING_REPLACE_ALL_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find_format_store.hpp>
#include <boost/string_algo/detail/replace_storage.hpp>

namespace boost {
    namespace string_algo {
        namespace detail {

// replace_all_copy (iterator variant) implementation ---------------------------//

            template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline OutputIteratorT replace_all_copy_impl(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult )
            {       
                return replace_all_copy_impl2( 
                    Output,
                    Input,
                    Finder,
                    Formatter,
                    FindResult,
                    Formatter(FindResult) );
            }

            template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline OutputIteratorT replace_all_copy_impl2(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult )
            {       
                typedef BOOST_STRING_TYPENAME 
                    container_const_iterator<InputT>::type input_iterator_type; 

                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                // Initialize last match
                input_iterator_type LastMatch=begin(Input);

                // Iterate throug all matches
                while( !M.empty() )
                {
                    // Copy the beginning of the sequence
                    std::copy( LastMatch, M.begin(), Output );
                    // Copy formated result
                    std::copy( begin(M.format_result()), end(M.format_result()), Output );

                    // Proceed to the next match
                    LastMatch=M.end();
                    M=Finder( LastMatch, end(Input) );
                }

                // Copy the rest of the sequence
                std::copy( LastMatch, end(Input), Output );

                return Output;
            }

// replace_all_copy implementation ----------------------------------------------//

            template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline InputT replace_all_copy_impl(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult)
            {
                return replace_all_copy_impl2(
                    Input,
                    Finder,
                    Formatter,
                    FindResult,
                    Formatter(FindResult) );
            }

            template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline InputT replace_all_copy_impl2(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult)
            {
                typedef BOOST_STRING_TYPENAME 
                    container_const_iterator<InputT>::type input_iterator_type; 

                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                // Initialize last match
                input_iterator_type LastMatch=begin(Input);

                // Output temporary
                InputT Output;

                // Iterate throug all matches
                while( !M.empty() )
                {
                    // Copy the beginning of the sequence
                    insert( Output, end(Output), LastMatch, M.begin() );
                    // Copy formated result
                    insert( Output, end(Output), M.format_result() );

                    // Proceed to the next match
                    LastMatch=M.end();
                    M=Finder( LastMatch, end(Input) );
                }

                // Copy the rest of the sequence
                insert( Output, end(Output), LastMatch, end(Input) );

                return Output;
            }

// replace_all implementation ------------------------------------------------//
        
            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline InputT& replace_all_impl( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                FindResultT FindResult)
            {
                return replace_all_impl2(
                    Input,
                    Finder,
                    Formatter,
                    FindResult,
                    Formatter(FindResult) );
            }

            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline InputT& replace_all_impl2( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                FindResultT FindResult,
                FormatResultT FormatResult)
            {
                typedef BOOST_STRING_TYPENAME 
                    container_iterator<InputT>::type input_iterator_type; 
                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );
          
                // Instantiate replacement storage
                std::deque<
                    BOOST_STRING_TYPENAME container_value_type<InputT>::type> Storage;

                // Initialize replacement iterators
                input_iterator_type InsertIt=begin(Input);
                input_iterator_type SearchIt=begin(Input);
                
                while ( !M.empty() )
                {
                    // process the segment
                    InsertIt=process_segment( 
                        Storage,
                        Input,
                        InsertIt,
                        SearchIt,
                        M.begin() );
                    
                    // Adjust search iterator
                    SearchIt=M.end();

                    // Copy formated replace to the storage
                    copy_to_storage( Storage, M.format_result() );

                    // Find range for a next match
                    M=Finder( SearchIt, end(Input) );
                }

                // process the last segment
                InsertIt=process_segment( 
                    Storage,
                    Input,
                    InsertIt,
                    SearchIt,
                    end(Input) );
                
                if ( Storage.empty() )
                {
                    // Truncate input
                    erase( Input, InsertIt, end(Input) );
                }
                else
                {
                    // Copy remaining data to the end of input
                    insert( Input, end(Input), Storage.begin(), Storage.end() );
                }

                return Input;
            }

        } // namespace detail
    } // namespace string_algo
} // namespace boost

#endif  // BOOST_STRING_REPLACE_ALL_DETAIL_HPP
