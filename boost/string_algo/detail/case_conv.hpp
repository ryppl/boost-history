//  Boost string_algo library string_funct.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CASE_CONV_DETAIL_HPP
#define BOOST_STRING_CASE_CONV_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <locale>

namespace boost {

    namespace string_algo {

        namespace detail {

//  case conversion functors -----------------------------------------------//

            // a tolower functor 
            struct to_lowerF 
            {
                // Constructor
                to_lowerF( const std::locale& Loc ) : m_Loc( Loc ) {}

                // Operation
                template<typename CharT>
				CharT operator ()( CharT Ch ) const
                {
                    return std::tolower( Ch, m_Loc );
                }
            private:
                const std::locale& m_Loc;
            };

            // a toupper functor 
            struct to_upperF
            {
                // Constructor
                to_upperF( const std::locale& Loc ) : m_Loc( Loc ) {}

                // Operation
				template<typename CharT>
                CharT operator ()( CharT Ch ) const
                {
                    return std::toupper( Ch, m_Loc );
                }
            private:
                const std::locale& m_Loc;
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_CASE_CONV_DETAIL_HPP
