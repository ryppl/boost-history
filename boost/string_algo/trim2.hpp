//  Boost string_algo library trim2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRIM2_HPP
#define BOOST_STRING_TRIM2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/detail/trim.hpp>
#include <boost/string_algo/classification.hpp>
#include <locale>

/*! \file
    Trim algorithms are used to remove trailing and leading spaces from a 
    sequence (string). This header container parametric versions of
    trim algoritms. Space characters are selected using a predicate (functor). 
    Functions take a selection predicate as a parameter, which is used to determine 
    if a charater is a space. Common predicates are provided in classification.hpp header.
*/

namespace boost {
    namespace string_algo {

//  left trim  -----------------------------------------------//

        //! Left trim
        /*!
            Remove all leading spaces from the input. Suplied predicate
            is used to detemine which character is a space.
            Result is copied to the given output iterator.

            \param Output A output iterarot to which the result will be copied
            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return An output iterator pointing just after last inserted character
        */
        template< typename OutputIteratorT, typename ContainerT, typename PredicateT >
        inline OutputIteratorT trim_left_copy( 
            OutputIteratorT Output,
            const ContainerT& Input,
            PredicateT IsSpace )
        {
            std::copy( 
                string_algo::detail::trim_begin( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace ),
                string_algo::end(Input),
                Output );

            return Output;
        }

        //! Left trim
        /*!
            Remove all leading spaces from the input. Suplied predicate
            is used to detemine which character is a space.
            Result is a trimmed copy if the input

            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return A trimmed copy if the input
        */
        template< typename ContainerT, typename PredicateT >
        inline ContainerT trim_left_copy( const ContainerT& Input, PredicateT IsSpace )
        {
            return ContainerT( 
                string_algo::detail::trim_begin( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace ),
                string_algo::end(Input) );
        }

        //! Left trim
        /*!
            Remove all leading spaces from the input. Suplied predicate
            is used to detemine which character is a space.
            Input sequence is modified in-place.

            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return A reference to the modified input
        */
        template< typename ContainerT, typename PredicateT >
        inline ContainerT& trim_left( ContainerT& Input, PredicateT IsSpace )
        {
            Input.erase( 
                string_algo::begin(Input),
                string_algo::detail::trim_begin( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace ));

            return Input;
        }

//  right trim  -----------------------------------------------//

        //! Right trim
        /*!
            Remove all trailing spaces from the input. Suplied predicate
            is used to detemine which character is a space.
            Result is copied to the given output iterator.

            \param Output A output iterarot to which the result will be copied
            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return An output iterator pointing just after last inserted character
        */
        template< typename OutputIteratorT, typename ContainerT, typename PredicateT >
        inline OutputIteratorT trim_right_copy( 
            OutputIteratorT Output,
            const ContainerT& Input,
            PredicateT IsSpace )
        {
            std::copy( 
                string_algo::begin(Input),
                string_algo::detail::trim_end( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace ),
                Output );

            return Output;
        }

        //! Right trim
        /*!
            Remove all trailing spaces from the input. Suplied predicate
            is used to detemine which character is a space.
            Result is a trimmed copy if the input

            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return A trimmed copy if the input
        */
        template< typename ContainerT, typename PredicateT >
        inline ContainerT trim_right_copy( const ContainerT& Input, PredicateT IsSpace )
        {
            return ContainerT( 
                string_algo::begin(Input),
                string_algo::detail::trim_end( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace )
                );
        }
        
        //! Right trim
        /*!
            Remove all trailing spaces from the input. Suplied predicate
            is used to detemine which character is a space.
            Input sequence is modified in-place.

            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return A reference to the modified input
        */
        template< typename ContainerT, typename PredicateT >
        inline ContainerT& trim_right( ContainerT& Input, PredicateT IsSpace )
        {
            Input.erase(
                string_algo::detail::trim_end( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace ),
                string_algo::end(Input)
                );

            return Input;
        }

//  both side trim  -----------------------------------------------//

        //! Trim
        /*!
            Remove all trailing and leading spaces from the input. 
            Suplied predicate is used to detemine which character is a space.
            Result is copied to the given output iterator.

            \param Output A output iterarot to which the result will be copied
            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return An output iterator pointing just after last inserted character
        */
        template< typename OutputIteratorT, typename ContainerT, typename PredicateT >
        inline OutputIteratorT trim_copy( 
            OutputIteratorT Output,
            const ContainerT& Input,
            PredicateT IsSpace )
        {
            BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<ContainerT>::type TrimEnd=
                string_algo::detail::trim_end( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    IsSpace);

            std::copy( 
                string_algo::detail::trim_begin( 
                    string_algo::begin(Input), TrimEnd, IsSpace ),
                TrimEnd,
                Output
                );

            return Output;
        }

        //! Trim
        /*!
            Remove all leading and trailing spaces from the input. 
            Suplied predicate is used to detemine which character is a space.
            Result is a trimmed copy if the input

            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return A trimmed copy if the input
        */
        template< typename ContainerT, typename PredicateT >
        inline ContainerT trim_copy( const ContainerT& Input, PredicateT IsSpace )
        {
            BOOST_STRING_TYPENAME 
                container_const_iterator<ContainerT>::type TrimEnd=
                    string_algo::detail::trim_end( 
                        string_algo::begin(Input), 
                        string_algo::end(Input), 
                        IsSpace);

            return ContainerT( 
                string_algo::detail::trim_begin( 
                    string_algo::begin(Input), 
                    TrimEnd, 
                    IsSpace ),
                TrimEnd
                );
        }
        
        //! Trim
        /*!
            Remove all leading and trailing spaces from the input. 
            Suplied predicate is used to detemine which character is a space.
            Input sequence is modified in-place.

            \param Input An input container
            \param IsSpace An unary predicate identifying spaces
            \return A reference to the modified input
        */
        template< typename ContainerT, typename PredicateT >
        inline ContainerT& trim( ContainerT& Input, PredicateT IsSpace )
        {
            return trim_left( trim_right( Input, IsSpace ), IsSpace );
        }

    } // namespace string_algo
} // namespace boost

#endif  // BOOST_STRING_TRIM2_HPP
