//  Boost string_algo library classification.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CLASSIFICATION_DETAIL_HPP
#define BOOST_STRING_CLASSIFICATION_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <algorithm>
#include <functional>
#include <locale>
#include <set>
#include <boost/string_algo/container_traits.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  classification functors -----------------------------------------------//
			
            // is_classified functor
			template< typename CharT >
			struct is_classifiedF :
				public std::unary_function< CharT, bool >
            {
                // Constructor from a locale 
                is_classifiedF(std::ctype_base::mask Type, std::locale const & Loc = std::locale()) :
                    m_Type(Type), m_Locale(Loc) {}

                // Operation
                bool operator()( CharT Ch ) const
                {
                    return std::use_facet< std::ctype<CharT> >(m_Locale).is( m_Type, Ch );
                }

            private:
                const std::ctype_base::mask m_Type;
                const std::locale m_Locale;
            };

			// an is_of functor 
            /* 
                returns true if the value is from the specitied range
            */
            template< typename CharT >
			struct is_ofF :
				std::unary_function< CharT, bool >
            {
                // Constructor 
				template< typename SeqT >
                is_ofF( const SeqT& Seq ) : m_Set( begin(Seq), end(Seq) ) {}
                
                // Operation
                bool operator()( CharT Ch ) const
                {
                    return m_Set.find( Ch )!=m_Set.end();
                }
            
            private:
                std::set<CharT> m_Set;                
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_CLASSIFICATION_DETAIL_HPP
