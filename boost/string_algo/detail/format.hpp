//  Boost string_algo library format.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FORMAT_DETAIL_HPP
#define BOOST_STRING_FORMAT_DETAIL_HPP

#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/util.hpp>

//  generic replace functors -----------------------------------------------//

namespace boost {
    namespace string_algo {
        namespace detail {

//  const format functor ----------------------------------------------------//

            // constant format functor
            template< typename FormatT >
            struct const_formatF
            {
                typedef BOOST_STRING_TYPENAME
                    container_const_iterator<FormatT>::type format_iterator;
                typedef iterator_range<format_iterator> result_type;

                // Construction
                const_formatF( const FormatT& Format ) :
                    m_Format( begin(Format), end(Format) ) {}

                // Operation
                template< typename ReplaceT >
                const result_type& operator()( const ReplaceT& ) const
                {
                    return m_Format;
                }

            private:
                result_type m_Format;
            };

//  identity format functor ----------------------------------------------------//

            // indentity format functor
            template< typename FormatT >
            struct identity_formatF
            {
                typedef FormatT result_type;

                // Operation
                template< typename ReplaceT >
                const result_type& operator()( const ReplaceT& Replace ) const
                {
                    return FormatT( begin(Replace), end(Replace) );
                }
            };

//  empty format functor ( used by erase ) ------------------------------------//
        
            // empty format functor
            template< typename CharT >
            struct empty_formatF
            {
                typedef empty_container<CharT> result_type;

                template< typename ReplaceT >
                result_type operator()( const ReplaceT& ) const
                {
                    return result_type();
                }
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FORMAT_DETAIL_HPP
