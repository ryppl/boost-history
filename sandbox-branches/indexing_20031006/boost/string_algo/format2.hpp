//  Boost string_algo library format2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FORMAT2_HPP
#define BOOST_STRING_FORMAT2_HPP

#include <boost/detail/iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {
    namespace string_algo {

// generic formaters  ---------------------------------------------------------------//

        // constant formatter
        template< typename ContainerT >
        inline detail::const_formatF<ContainerT>
        const_formatter( const ContainerT& Format )
        {
            return detail::const_formatF<ContainerT>( Format );
        }

        // identity formatter
        template< typename ContainerT >
        inline detail::identity_formatF<ContainerT>
        identity_formatter()
        {
            return detail::identity_formatF<ContainerT>();
        }

        // empty formatter
        template< typename ContainerT >
        inline detail::empty_formatF< 
            BOOST_STRING_TYPENAME container_traits<ContainerT>::value_type >
        empty_formatter( const ContainerT& Input )
        {
            return detail::empty_formatF<
                BOOST_STRING_TYPENAME container_traits<ContainerT>::value_type >();
        }


    } // namespace string_algo
} // namespace boost


#endif  // BOOST_FORMAT2_HPP
