//  Boost string_algo library format_regex.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FORMAT_REGEX_DETAIL_HPP
#define BOOST_STRING_FORMAT_REGEX_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <string>
#include <boost/regex.hpp>
#include <boost/string_algo/detail/find_regex.hpp>

namespace boost {

//  regex replace format -----------------------------------------------//

    namespace string_algo {
       
        namespace detail {

//  regex format functor -----------------------------------------//

            // regex format functor
            template<typename StringT>
            struct regex_formatF
            {
                typedef StringT result_type;
                typedef BOOST_STRING_DEDUCED_TYPENAME StringT::value_type char_type;
                typedef const result_type& result_reference_type;

                // Construction
                regex_formatF( const StringT& Fmt, unsigned int Flags=0 ) :
                    m_Fmt(Fmt), m_Flags( Flags ) {}

                template<typename InputIteratorT>
                result_type operator()( 
                    const regex_search_result<InputIteratorT>& Replace ) const
                {
                    return regex_format( 
                        Replace.match_results(), m_Fmt, m_Flags );                      
                }
            private:
                const StringT& m_Fmt;
                unsigned int m_Flags;
            };

            // Construction helper
            template< typename CharT, typename TraitsT, typename AllocT >
            inline regex_formatF< std::basic_string< CharT, TraitsT, AllocT> >
            create_regex_format( 
                const std::basic_string<CharT, TraitsT, AllocT>& Format,
                unsigned int Flags=0 )
            {
                return 
                    regex_formatF< std::basic_string<CharT, TraitsT, AllocT> >(
                        Format,
                        Flags );
            }
        
        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FORMAT_DETAIL_HPP
