//  Boost string_algo library replace2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE2_HPP
#define BOOST_STRING_REPLACE2_HPP

#include <deque>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/concept.hpp>
#include <boost/string_algo/detail/replace.hpp>
#include <boost/string_algo/detail/replace_all.hpp>

/*! \file
    This header defines generic replace algorithms. Each algorithm replaces
    a part(s) of the input. The part to be replaced is found using a Finder object.
    Result of finding is then used by a Formatter object to generate the replacement.
*/

namespace boost {
    namespace string_algo {

// generic replace  -----------------------------------------------------------------//

        //! Generic replace algorithm
        /*!
            Create new sequence into output interator as a copy of input sequence,
            but with the match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

            \param Output A output iterarot to which the result will be copied
            \param Input An input sequence
            \param Finder A Finder object used to search for a match to be replaced
            \param Formatter A Formatter object used to format a match
            \return An output iterator pointing just after last inserted character
        */
        template< 
            typename OutputIteratorT,
            typename InputT,
            typename FinderT,
            typename FormatterT >
        inline OutputIteratorT replace_copy(
            OutputIteratorT Output,
            const InputT& Input,
            FinderT Finder,
            FormatterT Formatter )
        {
            // Concept check
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();
            function_requires< 
                FormatterConcept<
                    FormatterT,
                    FinderT,BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();

            return detail::replace_copy_impl(
                Output,
                Input,
                Finder,
                Formatter,
                Finder( begin(Input), end(Input) ) );
        }

        //! Generic replace algorithm
        /*!
            Create new sequence as a copy the input sequence,
            but with the match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

            \param Input An input sequence
            \param Finder A Finder object used to search for a match to be replaced
            \param Formatter A Formatter object used to format a match
            \return An output iterator pointing just after last inserted character
        */
        template< 
            typename InputT, 
            typename FinderT,
            typename FormatterT >
        inline InputT replace_copy(
            const InputT& Input,
            FinderT Finder,
            FormatterT Formatter )
        {
            // Concept check
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();
            function_requires< 
                FormatterConcept<
                    FormatterT,
                    FinderT,BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();

            return detail::replace_copy_impl(
                Input,
                Finder,
                Formatter,
                Finder( begin(Input), end(Input) ) );
        }

        //! Generic replace algorithm
        /*!
            Replace a match in the input sequence with the formated substitute.
            The match is searched using find functor and substitute is formated using
            format functor.

            \param Input An input sequence
            \param Finder A Finder object used to search for a match to be replaced
            \param Formatter A Formatter object used to format a match
            \return An output iterator pointing just after last inserted character
        */
        template<
            typename InputT,
            typename FinderT,
            typename FormatterT >
        inline InputT& replace( 
            InputT& Input,
            FinderT Finder,
            FormatterT Formatter )
        {
            // Concept check
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();
            function_requires< 
                FormatterConcept<
                    FormatterT,
                    FinderT,BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();

            return detail::replace_impl(
                Input,
                Finder,
                Formatter,
                Finder( begin(Input), end(Input) ) );
        }


//  replace_all generic ----------------------------------------------------------------//

        //! Generic replace all algorithm
        /*!
            Create new sequence into output interator as a copy of input sequence,
            but with the each match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

            \param Output A output iterarot to which the result will be copied
            \param Input An input sequence
            \param Finder A Finder object used to search for a match to be replaced
            \param Formatter A Formatter object used to format a match
            \return An output iterator pointing just after last inserted character
        */
        template< 
            typename OutputIteratorT,
            typename InputT,
            typename FinderT,
            typename FormatterT >
        inline OutputIteratorT replace_all_copy(
            OutputIteratorT Output,
            const InputT& Input,
            FinderT Finder,
            FormatterT Formatter )
        {
            // Concept check
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();
            function_requires< 
                FormatterConcept<
                    FormatterT,
                    FinderT,BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();

            return detail::replace_all_copy_impl(
                Output,
                Input,
                Finder,
                Formatter,
                Finder( begin(Input), end(Input) ) );
        }

        //! Generic replace all algorithm
        /*!
            Create new sequence as a copy the input sequence,
            but with the each match replaced with the formated substitute range. 
            The match is searched using find functor and substitute is formated using
            format functor.

            \param Input An input sequence
            \param Finder A Finder object used to search for a match to be replaced
            \param Formatter A Formatter object used to format a match
            \return An output iterator pointing just after last inserted character
        */
        template< 
            typename InputT, 
            typename FinderT,
            typename FormatterT >
        inline InputT replace_all_copy(
            const InputT& Input,
            FinderT Finder,
            FormatterT Formatter )
        {
            // Concept check
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();
            function_requires< 
                FormatterConcept<
                    FormatterT,
                    FinderT,BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();

            return detail::replace_all_copy_impl(
                Input,
                Finder,
                Formatter,
                Finder( begin(Input), end(Input) ) );
        }

        //! Generic replace all algorithm
        /*!
            Replace each match in the input sequence with the formated substitute.
            The match is searched using find functor and substitute is formated using
            format functor.

            \param Input An input sequence
            \param Finder A Finder object used to search for a match to be replaced
            \param Formatter A Formatter object used to format a match
            \return An output iterator pointing just after last inserted character
        */
        template<
            typename InputT,
            typename FinderT,
            typename FormatterT >
        inline InputT& replace_all( 
            InputT& Input,
            FinderT Finder,
            FormatterT Formatter )
        {
            // Concept check
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();
            function_requires< 
                FormatterConcept<
                    FormatterT,
                    FinderT,BOOST_STRING_TYPENAME container_const_iterator<InputT>::type> >();

            return detail::replace_all_impl(
                Input,
                Finder,
                Formatter,
                Finder( begin(Input), end(Input) ) );

        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_REPLACE2_HPP
