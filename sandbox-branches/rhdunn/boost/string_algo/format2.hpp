//  Boost string_algo library format2.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FORMAT2_HPP
#define BOOST_STRING_FORMAT2_HPP

#include <boost/detail/iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/format.hpp>

/*! \file
    Defines formatter generators. Formatter is a functor which formats
    a string according to give paramters. A Formatter works
    in conjuntion with a Finder. A Finder can provide addtional information
    for a specific Formatter. An example of such a cooperation is reger_finder
    and regex_formatter.

    Formatters are used by as a plugable components for replace facilities. 
    This header contains generator functions for the Formatters provided in this library.
*/

namespace boost {
    namespace string_algo {

// generic formaters  ---------------------------------------------------------------//

        //! Constant formatter
        /*!
            Construct the \c const_formatter. Const formatter always returns
            the same value, regardless of the paramter.

            \param Format A predefined value used as a result for formating
            \return An instance of the \c const_formatter object.
        */
        template< typename ContainerT >
        inline detail::const_formatF<ContainerT>
        const_formatter( const ContainerT& Format )
        {
            return detail::const_formatF<ContainerT>( Format );
        }

        //! Identity formatter
        /*!
            Construct the \c identity_formatter. Identity formatter always returns
            the parameter.

            \return An instance of the \c identity_formatter object.
        */
        template< typename ContainerT >
        inline detail::identity_formatF<ContainerT>
        identity_formatter()
        {
            return detail::identity_formatF<ContainerT>();
        }

        //! Empty formatter
        /*!
            Construct the \c empty_formatter. Empty formater always returns an empty
            sequence. 

            \param Input container used to select a correct value_type for the
                         resulting empty_container<>.
            \return An instance of the \c empty_formatter object.
        */
        template< typename ContainerT >
        inline detail::empty_formatF< 
            BOOST_STRING_TYPENAME container_value_type<ContainerT>::type >
        empty_formatter( const ContainerT& Input )
        {
            return detail::empty_formatF<
                BOOST_STRING_TYPENAME container_value_type<ContainerT>::type >();
        }


    } // namespace string_algo
} // namespace boost


#endif  // BOOST_FORMAT2_HPP
