//  Boost string_algo library case_conv.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CASE_CONV_HPP
#define BOOST_STRING_CASE_CONV_HPP

#include <boost/string_algo/config.hpp>
#include <algorithm>
#include <locale>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/detail/case_conv.hpp>

/*! \file boost/string_algo/case_conv.hpp
	Sequence case conversion algorithms are located in this header file.
	Algorithm convert each element in the input sequence to the
	desired case using provided locales.
*/

namespace boost {

//  to_lower  -----------------------------------------------//

    //! Convert to lower case
    /*!
        Each element of the input sequence is converted to lower
        case. Result is copied to the given output iterator.

		\param Output A output iterarot to which the result will be copied
		\param Input An input container
		\param Loc A locale used for conversion
		\return An output iterator pointing just after last inserted character
    */
    template< typename OutputIteratorT, typename ContainerT >
    inline OutputIteratorT 
    to_lower_copy(
        OutputIteratorT Output,
        const ContainerT& Input,
        const std::locale& Loc=std::locale() )
    {
        return std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            Output,
            string_algo::detail::to_lowerF(Loc) );
    }

    //! Convert to lower case
    /*!
        Each element of the input sequence is converted to lower
        case. Result is a copy of the input converted to lower case.

		\param Input An input container
		\param Loc a locale used for conversion
		\return A copy if the input converted to lower case
    */
    template< typename ContainerT >
    inline ContainerT to_lower_copy( 
		const ContainerT& Input, 
		const std::locale& Loc=std::locale() )
    {
        ContainerT Output;
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            std::back_inserter<ContainerT>( Output ),
            string_algo::detail::to_lowerF(Loc) );

        return Output;
    }

    //! Convert to lower case
    /*!
        Each element of the input sequence is converted to lower
        case. Input sequence is modified in-place.

		\param Input An input container
		\param Loc a locale used for conversion
		\return A reference to the modified input
    */
    template< typename ContainerT >
    inline ContainerT& to_lower( 
		ContainerT& Input, 
		const std::locale& Loc=std::locale() )
    {
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            string_algo::begin(Input), 
            string_algo::detail::to_lowerF(Loc) );
        
        return Input;
    }
    
//  to_upper  -----------------------------------------------//

    //! Convert to upper case
    /*!
        Each element of the input sequence is converted to upper
        case. Result is copied to the given output iterator.

		\param Output A output iterarot to which the result will be copied
		\param Input An input container
		\param Loc a locale used for conversion
		\return An output iterator pointing just after last inserted character
    */
    template< typename OutputIteratorT, typename ContainerT >
    inline OutputIteratorT 
    to_upper_copy(
        OutputIteratorT Output,
        const ContainerT& Input,
        const std::locale& Loc=std::locale() )
    {
        return std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            Output,
            string_algo::detail::to_upperF(Loc) );
    }

    //! Convert to upper case
    /*!
        Each element of the input sequence is converted to upper
        case. Result is a copy if the input converted to upper case.

		\param Input An input container
		\param Loc a locale used for conversion
		\return A copy if the input converted to upper case
    */
    template< typename ContainerT >
    inline ContainerT to_upper_copy( 
		const ContainerT& Input, 
		const std::locale& Loc=std::locale() )
    {
        ContainerT Output;
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            std::back_inserter<ContainerT>( Output ),
            string_algo::detail::to_upperF(Loc) );

        return Output;
    }

    //! Convert to upper case
    /*!
        Each element of the input sequence is converted to upper
        case. Input sequence is modified in-place.

		\param Input An input container
		\param Loc a locale used for conversion
		\return A reference to the modified input
    */
    template< typename ContainerT >
    inline ContainerT& to_upper( 
		ContainerT& Input, 
		const std::locale& Loc=std::locale() )
    {
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            string_algo::begin(Input), 
            string_algo::detail::to_upperF(Loc) );
        
        return Input;
    }

} // namespace boost


#endif  // BOOST_STRING_CASE_CONV_HPP
