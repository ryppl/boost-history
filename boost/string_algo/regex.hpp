//  Boost string_algo library regex.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REGEX_HPP
#define BOOST_STRING_REGEX_HPP

#include <boost/regex.hpp>
#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/find_impl.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/detail/find_regex.hpp>
#include <boost/string_algo/detail/format_regex.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {

//  find_regex  -----------------------------------------------//

    // find the first match a subsequnce in the sequence
    template< 
        typename InputIteratorT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline string_algo::detail::regex_search_result<InputIteratorT>
    find_regex( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        // Forward the call to the functor
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_regexF< iterator_range<InputIteratorT> >::
                    create_const( Rx, MatchFlags ) );
    }

    // find_first sequence const version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline string_algo::detail::
        regex_search_result< 
			typename string_algo::input_policy<InputT>::iterator_type >
    find_regex( 
        InputT& Input, 
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::find_regexF<
				InputT,
				reg_expression<CharT, RegexTraitsT, RegexAllocatorT>,
				typename string_algo::input_policy<InputT>::policy >( Rx, MatchFlags ) );
    }

//  replace_regex --------------------------------------------------------------------//

    // replace_regex iterator version
    template< 
        typename InputIteratorT,
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_regex_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_copy( 
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_regexF< iterator_range<InputIteratorT> >::
                    create_const( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_copy( 
            Output,
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::create_const( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

    // replace_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    InputT replace_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::create_const( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

    // replace_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    InputT& replace_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace( 
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::create( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

//  replace_all_regex --------------------------------------------------------------------//

    // replace_all_regex iterator version
    template< 
        typename InputIteratorT,
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_regex_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_all_copy( 
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_regexF< iterator_range<InputIteratorT> >::
                    create_const( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

    // replace_all_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_all_copy( 
            Output,
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::
                    create_const( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

    // replace_all_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    InputT replace_all_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::
                    create_const( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

    // replace_all_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    InputT& replace_all_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_all( 
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::create( Rx, MatchFlags ),
            string_algo::detail::
                create_regex_formatF( Format, FormatFlags ) );
    }

//  erase_regex --------------------------------------------------------------------//

    // erase_regex iterator version
    template< 
        typename InputIteratorT,
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_regex_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_copy( 
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_regexF< iterator_range<InputIteratorT> >::
                    create_const( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<
                typename boost::detail::iterator_traits<InputIteratorT>::value_type >() );
    }

    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::create_const( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<typename InputT::value_type>() );
    }

    // erase_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    InputT erase_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::detail::
                create_find_regexF< InputT >::create_const( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<typename InputT::value_type>() );
    }

    // erase_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    InputT& erase_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_regexF< InputT >::create( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<typename InputT::value_type>() );
    }

//  erase_all_regex --------------------------------------------------------------------//

    // erase_all_regex iterator version
    template< 
        typename InputIteratorT,
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_all_regex_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all_copy( 
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_regexF< iterator_range<InputIteratorT> >::
                    create_const( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<
                typename boost::detail::iterator_traits<InputIteratorT>::value_type >() );
    }

    // erase_all_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_all_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_regexF< InputT >::create_const( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<typename InputT::value_type>() );
    }

    // erase_all_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    InputT erase_all_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::detail::
                create_find_regexF< InputT >::create_const( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<typename InputT::value_type>() );
    }

    // erase_all_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    InputT& erase_all_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::detail::
                create_find_regexF< InputT >::create( Rx, MatchFlags ),
            string_algo::detail::empty_formatF<typename InputT::value_type>() );
    }

} // namespace boost


#endif  // BOOST_STRING_REGEX_HPP
