//  Boost string_algo library regex2.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REGEX2_HPP
#define BOOST_STRING_REGEX2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/regex.hpp>
#include <boost/string_algo/detail/find_regex.hpp>
#include <boost/string_algo/detail/format_regex.hpp>

/*! \file
    Defines the \c refex_finder and \c regex_formatter generators. These two functors
    are designed to work together. \c regex_formatter uses addtional information
    about a match contained in the reger_finder search result.
*/

namespace boost {
    namespace string_algo {

//  regex_finder  -----------------------------------------------//

        //! "Regex" finder 
        /*!
            Construct the \c regex_finder. Finder uses regex engine to search
            for a match.
            Result is given in \c regex_search_result. This is an extension
            of the iterator_range. In addtion it containes match results 
            from the \c regex_search algoritm.

            \param Rx A regular expression
            \param MatchFlags Regex search options
            \return An instance of the \c regex_finder object
        */
        template< 
            typename CharT, 
            typename RegexTraitsT, typename RegexAllocatorT>
        inline detail::find_regexF< reg_expression<CharT, RegexTraitsT, RegexAllocatorT> >
        regex_finder(
            const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
            match_flag_type MatchFlags=match_default )
        {
            return detail::
                find_regexF< 
                    reg_expression<CharT, RegexTraitsT, RegexAllocatorT> >( Rx, MatchFlags );
        }

//  regex_formater  ---------------------------------------------//

        //! Regex formatter
        /*!
            Construct the \c regex_formatter. Regex formatter uses regex engine to
            format a match found by the \c regex_finder. 
            This formatted it designed to closely cooperate with \c regex_finder.

            \param Format Regex format definition
            \param Flags Format flags
            \return An instance of the \c regex_formatter functor
        */
       template< 
            typename CharT, 
            typename TraitsT, typename AllocT >
        inline detail::regex_formatF< std::basic_string< CharT, TraitsT, AllocT > >
        regex_formatter( 
            const std::basic_string<CharT, TraitsT, AllocT>& Format,
            match_flag_type Flags=format_default )
        {
            return 
                detail::regex_formatF< std::basic_string<CharT, TraitsT, AllocT> >(
                    Format,
                    Flags );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_REGEX2_HPP
