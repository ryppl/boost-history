//  Boost string_algo library regex2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REGEX2_HPP
#define BOOST_STRING_REGEX2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/regex.hpp>
#include <boost/string_algo/detail/find_regex.hpp>
#include <boost/string_algo/detail/format_regex.hpp>

namespace boost {
    namespace string_algo {

//  regex_finder  -----------------------------------------------//

        // regex finder
        template< 
            typename CharT, 
            typename RegexTraitsT, typename RegexAllocatorT>
        inline detail::find_regexF< reg_expression<CharT, RegexTraitsT, RegexAllocatorT> >
        regex_finder(
            const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
            unsigned int MatchFlags=match_default )
        {
            return detail::
                find_regexF< 
                    reg_expression<CharT, RegexTraitsT, RegexAllocatorT> >( Rx, MatchFlags );
        }

//  regex_formater  ---------------------------------------------//

        // regex formater
        template< 
            typename CharT, 
            typename TraitsT, typename AllocT >
        inline detail::regex_formatF< std::basic_string< CharT, TraitsT, AllocT > >
        regex_formatter( 
            const std::basic_string<CharT, TraitsT, AllocT>& Format,
            unsigned int Flags=0 )
        {
            return 
                detail::regex_formatF< std::basic_string<CharT, TraitsT, AllocT> >(
                    Format,
                    Flags );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_REGEX2_HPP
