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

/*! \file
	Defines formatter generators. Formatter is a functor which formats
	a string according to give paramters. Formatter should can work
	in conjuntion with finders. A finder can provide addtional information
	for a specific formatter. An example of such a cooperation is reger_finder
	and regex_formatter.

	Formatter are used by as a plugable components for replace facilities. 
	This header contains generator functions for formatters provided in this library.
*/

namespace boost {
    namespace string_algo {

// generic formaters  ---------------------------------------------------------------//

        //! Constant formatter
		/*!
			Construct a const formatter. Const formatter always returns
			the same value, regardless of the paramter.

			\param Format A predefined value used as a result for formating
			\return A const formatter functor
		*/
        template< typename ContainerT >
        inline detail::const_formatF<ContainerT>
        const_formatter( const ContainerT& Format )
        {
            return detail::const_formatF<ContainerT>( Format );
        }

        //! Identity formatter
		/*!
			Construct an identity formatter. Identity formatter always returns
			the parameter.

			\return An identity formatter functor
		*/
        template< typename ContainerT >
        inline detail::identity_formatF<ContainerT>
        identity_formatter()
        {
            return detail::identity_formatF<ContainerT>();
        }

        //! Empty formatter
		/*!
			Construct an empty formatter. Empty formater always returns an empty
			sequence. 

			\param Input container used to select a correct value_type for the
					     resulting empty_container<>
			\return An empty formatter functor
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
