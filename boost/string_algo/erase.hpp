//  Boost string_algo library erase.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_HPP
#define BOOST_STRING_ERASE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace2.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/format2.hpp>

/*! \file
    Defines various erase algorithms. Each algorithm removes
    a part(s) of the input according to a searching criterium.
*/

namespace boost {

//  erase_range -------------------------------------------------------//

    //! Erase range algorithm
    /*!
        Remove the given range from the input sequence.
        The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param SearchRange A range in the input to be substituted
        \return An output iterator pointing just after last inserted character
    */
    template< typename OutputIteratorT, typename InputT >
    inline OutputIteratorT erase_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputT>::type>& SearchRange )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::empty_formatter(Input) );
    }

    //! Erase range algorithm
    /*!
        Remove the given range from the input sequence.
        The result is a modified copy of the input.

        \param Input An input sequence
        \param SearchRange A range in the input to be substituted
        \return A modified copy of the input
    */
    template< typename InputT >
    inline InputT erase_range_copy( 
        const InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputT>::type>& SearchRange )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::empty_formatter(Input) );
    }

    //! Erase range algorithm
    /*!
        Remove the given range from the input sequence.
        The input sequence is modified in-place.

        \param Input An input sequence
        \param SearchRange A range in the input to be substituted
        \return A reference to the modified input
    */
    template< typename InputT >
    inline InputT& erase_range( 
        InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_iterator<InputT>::type>& SearchRange )
    {
        return string_algo::replace( 
            Input, 
            string_algo::range_finder(SearchRange),
            string_algo::empty_formatter(Input) );
    }

//  erase_first  --------------------------------------------------------//

    //! Erase first algorithm
    /*!
        Remove the first match of the search sequence from the input.
        The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    //! Erase first algorithm
    /*!
        Remove the first match of the search sequence from the input. 
        The result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT erase_first_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    //! Erase first algorithm
    /*!
        Remove the first match of the search sequence from the input. 
        The input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& erase_first( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

//  erase_first ( case insensitive ) ------------------------------------//

    //! Erase first algorithm ( case insensitive )
    /*!
        Remove the first match of the search sequence from the input. 
        The result is copied to the given output iterator. Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase first algorithm ( case insensitive )
    /*!
        Remove the first match of the search sequence from the input. 
        The result is a modified copy of the input. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for.
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT ierase_first_copy( 
        const InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase first algorithm ( case insensitive )
    /*!
        Remove the first match of the search sequence from the input. 
        The input sequence is modified in-place. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for.
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& ierase_first( 
        InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

//  erase_last  --------------------------------------------------------//

    //! Erase last algorithm
    /*!
        Remove the last match of the search sequence from the input. 
        The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    //! Erase last algorithm
    /*!
        Remove the last match of the search sequence from the input. 
        The result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT erase_last_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::last_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    //! Erase last algorithm
    /*!
        Remove the last match of the search sequence from the input. 
        The input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& erase_last( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder(Search),
            string_algo::empty_formatter(Input) );
    }

//  erase_last ( case insensitive ) ------------------------------------//

    //! Erase last algorithm ( case insensitive )
    /*!
        Remove the last match of the search sequence from the input. 
        The result is copied to the given output iterator. Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase last algorithm ( case insensitive )
    /*!
        Remove the last match of the search sequence from the input. 
        The result is a modified copy of the input. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for.
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT ierase_last_copy( 
        const InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::last_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase last algorithm ( case insensitive )
    /*!
        Remove the last match of the search sequence from the input. 
        The input sequence is modified in-place. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for.
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& ierase_last( 
        InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

//  erase_nth --------------------------------------------------------------------//

    //! Erase nth algorithm
    /*!
        Remove the Nth match of the search sequence in the input.
        The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder(Search, Nth),
            string_algo::empty_formatter(Input) );
    }

    //! Erase nth algorithm
    /*!
        Remove the Nth match of the search sequence in the input.
        The result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT erase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::nth_finder(Search, Nth),
            string_algo::empty_formatter(Input) );
    }

    //! Erase nth algorithm
    /*!
        Remove the Nth match of the search sequence in the input.
        The input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& erase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder(Search, Nth),
            string_algo::empty_formatter(Input) );
    }

//  erase_nth ( case insensitive ) ---------------------------------------------//

    //! Erase nth algorithm ( case insensitive )
    /*!
        Remove the Nth match of the search sequence in the input.
        The result is copied to the given output iterator. Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase nth algorithm
    /*!
        Remove the Nth match of the search sequence in the input.
        The result is a modified copy of the input. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT ierase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase nth algorithm
    /*!
        Remove the Nth match of the search sequence in the input.
        The input sequence is modified in-place. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& ierase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }


//  erase_all  --------------------------------------------------------//

    //! Erase all algorithm
    /*!
        Remove all the occurences of the search sequence from the input. 
        The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    //! Erase all algorithm
    /*!
        Remove all the occurences of the search sequence from the input. 
        The result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \return A modified copy of the input
    */  
    template< typename InputT, typename SearchT >
    inline InputT erase_all_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    //! Erase all algorithm
    /*!
        Remove all the occurences of the search sequence from the input. 
        The input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& erase_all( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

//  erase_all ( case insensitive ) ------------------------------------//

    //! Erase all algorithm ( case insensitive )
    /*!
        Remove all the occurences of the search sequence from the input. 
        The result is copied to the given output iterator. Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase all algorithm ( case insensitive )
    /*!
        Remove all the occurences of the search sequence from the input. 
        The result is a modified copy of the input. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputT, typename SearchT >
    inline InputT ierase_all_copy( 
        const InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

    //! Erase all algorithm ( case insensitive )
    /*!
        Remove all the occurences of the search sequence from the input. 
        The input sequence is modified in-place. Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputT, typename SearchT >
    inline InputT& ierase_all( 
        InputT& Input,
        const SearchT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::empty_formatter(Input) );
    }

//  erase_head --------------------------------------------------------------------//

    //! Erase head algorithm
    /*!
        Remove the head from the input. Head is a prefix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the head. The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param N A length of the head
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT >
    inline OutputIteratorT erase_head_copy(
        OutputIteratorT Output,
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::head_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase head algorithm
    /*!
        Remove the head from the input. Head is a prefix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the head. The result is a modified copy of the input.

        \param Input An input sequence
        \param N A length of the head
        \return A modified copy of the input
    */
    template< typename InputT >
    inline InputT erase_head_copy( 
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::head_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase head algorithm
    /*!
        Remove the head from the input. Head is a prefix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the head. The input sequence is modified in-place.

        \param Input An input sequence
        \param N A length of the head
        \return A reference to the modified input
    */
    template< typename InputT >
    inline InputT& erase_head( 
        InputT& Input,
        unsigned int N )
    {
        return string_algo::replace( 
            Input, 
            string_algo::head_finder(N),
            string_algo::empty_formatter( Input ) );
    }

//  erase_tail --------------------------------------------------------------------//

    //! Erase tail algorithm
    /*!
        Remove the tail from the input. Tail is a suffix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the tail. The result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param N A length of the head
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputT >
    inline OutputIteratorT erase_tail_copy(
        OutputIteratorT Output,
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::tail_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase tail algorithm
    /*!
        Remove the tail from the input. Tail is a suffix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence is 
        considered to be the tail. The result is a modified copy of the input.

        \param Input An input sequence
        \param N A length of the head
        \return A modified copy of the input
    */
    template< typename InputT >
    inline InputT erase_tail_copy( 
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::tail_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase tail algorithm
    /*!
        Remove the tail from the input. Tail is a suffix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence is
        considered to be the tail. The input sequence is modified in-place.

        \param Input An input sequence
        \param N A length of the head
        \return A reference to the modified input
    */
    template< typename InputT >
    inline InputT& erase_tail( 
        InputT& Input,
        unsigned int N )
    {
        return string_algo::replace( 
            Input, 
            string_algo::tail_finder(N),
            string_algo::empty_formatter( Input ) );
    }
    
} // namespace boost


#endif  // BOOST_ERASE_HPP
