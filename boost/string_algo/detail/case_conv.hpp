//  Boost string_algo library string_funct.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

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
