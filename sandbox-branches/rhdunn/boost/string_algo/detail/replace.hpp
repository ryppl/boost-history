//  Boost string_algo library replace.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_DETAIL_HPP
#define BOOST_STRING_REPLACE_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find_format_store.hpp>
#include <boost/string_algo/detail/replace_storage.hpp>

namespace boost {
    namespace string_algo {
        namespace detail {

// replace_copy (iterator variant) implementation -------------------------------//

            template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline OutputIteratorT replace_copy_impl(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult )
            {       
                return replace_copy_impl2( 
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
            inline OutputIteratorT replace_copy_impl2(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult )
            {       
                typedef find_format_store<
                    BOOST_STRING_TYPENAME 
                        container_const_iterator<InputT>::type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

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

// replace_copy implementation --------------------------------------------------//

            template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline InputT replace_copy_impl(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult)
            {
                return replace_copy_impl2(
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
            inline InputT replace_copy_impl2(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult)
            {
                typedef find_format_store<
                    BOOST_STRING_TYPENAME 
                        container_const_iterator<InputT>::type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                if ( M.empty() )
                {
                    // Match not found - return original sequence
                    return InputT( Input );
                }

                InputT Output;
                // Copy the beginning of the sequence
                insert( Output, end(Output), begin(Input), M.begin() );
                // Copy formated result
                insert( Output, end(Output), M.format_result() );
                // Copy the rest of the sequence
                insert( Output, end(Output), M.end(), end(Input) );

                return Output;
            }

// replace implementation ----------------------------------------------------//
        
            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline InputT& replace_impl( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult)
            {
                return replace_impl2(
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
            inline InputT& replace_impl2( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult)
            {
                typedef find_format_store<
                    BOOST_STRING_TYPENAME 
                        container_iterator<InputT>::type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                if ( M.empty() )
                {
                    // Search not found - return original sequence
                    return Input;
                }

                // Replace match
                replace( Input, M.begin(), M.end(), M.format_result() );
                
                return Input;
            }

        } // namespace detail
    } // namespace string_algo
} // namespace boost

#endif  // BOOST_STRING_REPLACE_DETAIL_HPP
