//  Boost string_algo library format.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FORMAT_DETAIL_HPP
#define BOOST_STRING_FORMAT_DETAIL_HPP

#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/detail/util.hpp>

namespace boost {

//  generic replace functors -----------------------------------------------//

    namespace string_algo {
        
        namespace detail {

//  replace identity format functor -----------------------------------------//

            // identity format functor
            template< typename FormatT >
            struct identity_formatF
            {
                typedef FormatT result_type;
                typedef const result_type& result_reference_type;
                
                // Construction
                identity_formatF( const FormatT& Format ) :
                    m_Format( Format ) {}

                // Operation
                template< typename ReplaceT >
                result_reference_type operator()( const ReplaceT& ) const
                {
                    return m_Format;
                }

            private:
                const FormatT& m_Format;
            };

//  empty format functor ( used by erase ) ------------------------------------//
        
            // empty format functor
            template< typename CharT >
            struct empty_formatF
            {
                typedef empty_container<CharT> result_type;
                typedef const result_type& result_reference_type;

                template< typename ReplaceT >
                result_type operator()( ReplaceT ) const
                {
                    return result_type();
                }
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FORMAT_DETAIL_HPP
