//  Boost string_algo library regex.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REGEX_HPP
#define BOOST_STRING_REGEX_HPP

#include <boost/string_algo/config.hpp>
#include <boost/regex.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace2.hpp>
#include <boost/string_algo/regex2.hpp>
#include <boost/string_algo/format2.hpp>
#include <boost/string_algo/split2.hpp>

namespace boost {

//  find_regex  -----------------------------------------------//

    // find the first match a subsequnce in the sequence
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator >
    find_regex( 
        InputT& Input, 
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::regex_finder(Rx,MatchFlags)( Input );
    }

    // find the first match a subsequnce in the sequence ( copy version )
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline InputT find_regex_copy( 
        const InputT& Input, 
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return copy_range<InputT>(string_algo::regex_finder(Rx,MatchFlags)(Input) );
    }

//  replace_regex --------------------------------------------------------------------//

    // replace_regex output-iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
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
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    // replace_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT replace_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    // replace_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT& replace_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

//  replace_all_regex --------------------------------------------------------------------//

    // replace_all_regex iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
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
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    // replace_all_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT replace_all_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    // replace_all_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT& replace_all_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        unsigned int MatchFlags=match_default,
        unsigned int FormatFlags=0 )
    {
        return string_algo::replace_all( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

//  erase_regex --------------------------------------------------------------------//

    // erase_regex output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline OutputIteratorT erase_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    // erase_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline InputT erase_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    // erase_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline InputT& erase_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

//  erase_all_regex --------------------------------------------------------------------//

    // erase_all_regex sequence version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline OutputIteratorT erase_all_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    // erase_all_regex sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline InputT erase_all_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    // erase_all_regex in-place sequence version
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline InputT& erase_all_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

//  find_all_regex ------------------------------------------------------------------//

    template< 
        typename ResultT, 
        typename InputT,         
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline ResultT& find_all_regex(
        ResultT& Result,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::iter_find(
            Result,
            Input,
            string_algo::regex_finder(Rx,MatchFlags) );         
    }

//  split_regex ------------------------------------------------------------------//

    template< 
        typename ResultT, 
        typename InputT,         
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline ResultT& split_regex(
        ResultT& Result,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        unsigned int MatchFlags=match_default )
    {
        return string_algo::iter_split(
            Result,
            Input,
            string_algo::regex_finder(Rx,MatchFlags) );         
    }

} // namespace boost


#endif  // BOOST_STRING_REGEX_HPP
