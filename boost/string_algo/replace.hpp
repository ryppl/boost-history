//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace2.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/format2.hpp>
#include <boost/string_algo/compare.hpp>

/*! \file
    This header defines various replace algorithms. Each algorithm replaces
    a part(s) of the input according to a searching and a replace criterium.
*/

namespace boost {

//  replace_range --------------------------------------------------------------------//

    //! Replace range algorithm
    /*!
        Substitute the given range in the input sequence with the
        format seqeuence. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param SearchRange A range in the input to be substituted
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename FormatSeqT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputSeqT>::type>& SearchRange,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::const_formatter(Format) );
    }

    //! Replace range algorithm
    /*!
        Substitute a given range in the input sequence with the
        format seqeuence. Result is a modified copy of the input.

        \param Input An input sequence
        \param SearchRange A range in the input to be substituted
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename FormatSeqT >
    inline InputSeqT replace_range_copy( 
        const InputSeqT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputSeqT>::type>& SearchRange,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::const_formatter(Format) );
    }

    //! Replace range algorithm
    /*!
        Substitute a given range in the input sequence with the
        format seqeuence. Input sequence is modified in-place.

        \param Input An input sequence
        \param SearchRange A range in the input to be substituted
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename FormatSeqT >
    inline InputSeqT& replace_range( 
        InputSeqT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_iterator<InputSeqT>::type>& SearchRange,
        const FormatSeqT& Format )
    {
        return string_algo::replace(
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::const_formatter(Format) );
    }

//  replace_first --------------------------------------------------------------------//

    //! Replace first algorithm
    /*!
        Substitute a first match of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder(Search),
            string_algo::const_formatter(Format) );
    }

    //! Replace first algorithm
    /*!
        Substitute a first match of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT replace_first_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::first_finder(Search),
            string_algo::const_formatter(Format) );
    }

    //! Replace first algorithm
    /*!
        Substitute a first match of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& replace_first( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::const_formatter(Format) );
    }

//  replace_first ( case insensitive ) ---------------------------------------------//

    //! Replace first algorithm ( case insensitive )
    /*!
        Substitute a first match of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT ireplace_first_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace first algorithm ( case insensitive )
    /*!
        Substitute a first match of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.
        Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT ireplace_first_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace first algorithm ( case insensitive )
    /*!
        Substitute a first match of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& ireplace_first( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

//  replace_last --------------------------------------------------------------------//

    //! Replace last algorithm
    /*!
        Substitute a last match of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder(Search),
            string_algo::const_formatter(Format) );
    }

    //! Replace last algorithm
    /*!
        Substitute a last match of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT replace_last_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::last_finder(Search),
            string_algo::const_formatter(Format) );
    }

    //! Replace last algorithm
    /*!
        Substitute a last match of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& replace_last( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder(Search),
            string_algo::const_formatter(Format) );
    }

//  replace_last ( case insensitive ) -----------------------------------------------//

    //! Replace last algorithm ( case insensitive )
    /*!
        Substitute a last match of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.
        Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT ireplace_last_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace last algorithm ( case insensitive )
    /*!
        Substitute a last match of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT ireplace_last_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::last_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace last algorithm ( case insensitive )
    /*!
        Substitute a last match of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& ireplace_last( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

//  replace_nth --------------------------------------------------------------------//

    //! Replace nth algorithm
    /*!
        Substitute an Nth match of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT replace_nth_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        unsigned int Nth,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder(Search, Nth),
            string_algo::const_formatter(Format) );
    }

    //! Replace nth algorithm
    /*!
        Substitute an Nth match of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT replace_nth_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        unsigned int Nth,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::nth_finder(Search, Nth),
            string_algo::const_formatter(Format) );
    }

    //! Replace nth algorithm
    /*!
        Substitute an Nth match of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& replace_nth( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        unsigned int Nth,
        const FormatSeqT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder(Search, Nth),
            string_algo::const_formatter(Format) );
    }

//  replace_nth ( case insensitive ) -----------------------------------------------//
    
    //! Replace nth algorithm ( case insensitive )
    /*!
        Substitute an Nth match of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.
        Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT ireplace_nth_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        unsigned int Nth,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal(Loc) ),
            string_algo::const_formatter(Format) );
    }

    //! Replace nth algorithm ( case insensitive )
    /*!
        Substitute an Nth match of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT ireplace_nth_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        unsigned int Nth,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace nth algorithm ( case insensitive )
    /*!
        Substitute an Nth match of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Nth An index of the match to be replaced
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& ireplace_nth( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        unsigned int Nth,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

//  replace_all --------------------------------------------------------------------//

    //! Replace all algorithm
    /*!
        Substitute all occurences of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder(Search),
            string_algo::const_formatter(Format) );
    }

    //! Replace all algorithm
    /*!
        Substitute all occurences of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT replace_all_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::first_finder(Search),
            string_algo::const_formatter(Format) );
    }

    //! Replace all algorithm
    /*!
        Substitute all occurences of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& replace_all( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::const_formatter(Format) );
    }
    
//  replace_all ( case insensitive ) -----------------------------------------------//

    //! Replace all algorithm ( case insensitive )
    /*!
        Substitute all occurences of the search sequence in the input 
        with the format seqeuence. Result is copied to the given output iterator.
        Searching is case insensitive.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename SearchSeqT,
        typename FormatSeqT >
    inline OutputIteratorT ireplace_all_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace all algorithm ( case insensitive )
    /*!
        Substitute all occurences of the search sequence in the input 
        with the format seqeuence. Result is a modified copy of the input.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT ireplace_all_copy( 
        const InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }

    //! Replace all algorithm ( case insensitive )
    /*!
        Substitute all occurences of the search sequence in the input 
        with the format seqeuence. Input sequence is modified in-place.
        Searching is case insensitive.

        \param Input An input sequence
        \param Search A string to be searched for. 
        \param Format A substitute sequence
        \param Loc a locale used for case insensitive comparison
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename SearchSeqT, typename FormatSeqT >
    inline InputSeqT& ireplace_all( 
        InputSeqT& Input,
        const SearchSeqT& Search,
        const FormatSeqT& Format,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal(Loc)),
            string_algo::const_formatter(Format) );
    }
    
//  replace_head --------------------------------------------------------------------//

    //! Replace head algorithm
    /*!
        Replace the head of the input with the given format sequence. 
        Head is a prefix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the head. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param N A length of the head
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename FormatSeqT >
    inline OutputIteratorT replace_head_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        unsigned int N,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::head_finder(N),
            string_algo::const_formatter(Format) );
    }

    //! Replace head algorithm
    /*!
        Replace the head of the input with the given format sequence. 
        Head is a prefix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the head. Result is a modified copy of the input.

        \param Input An input sequence
        \param N A length of the head
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename FormatSeqT >
    inline InputSeqT replace_head_copy( 
        const InputSeqT& Input,
        unsigned int N,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::head_finder(N),
            string_algo::const_formatter(Format) );
    }

    //! Replace head algorithm
    /*!
        Replace the head of the input with the given format sequence. 
        Head is a prefix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the head. Input sequence is modified in-place.

        \param Input An input sequence
        \param N A length of the head
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename FormatSeqT >
    inline InputSeqT& replace_head( 
        InputSeqT& Input,
        unsigned int N,
        const FormatSeqT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::head_finder(N),
            string_algo::const_formatter(Format) );
    }

//  replace_tail --------------------------------------------------------------------//

    //! Replace tail algorithm
    /*!
        Replace the tail of the input with the given format sequence. 
        Tail is a suffix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the tail. Result is copied to the given output iterator.

        \param Output A output iterarot to which the result will be copied
        \param Input An input sequence
        \param N A length of the head
        \param Format A substitute sequence
        \return An output iterator pointing just after last inserted character
    */
    template< 
        typename OutputIteratorT,
        typename InputSeqT, 
        typename FormatSeqT >
    inline OutputIteratorT replace_tail_copy(
        OutputIteratorT Output,
        const InputSeqT& Input,
        unsigned int N,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::tail_finder(N),
            string_algo::const_formatter(Format) );
    }

    //! Replace tail algorithm
    /*!
        Replace the tail of the input with the given format sequence. 
        Tail is a suffix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the tail. Result is a modified copy of the input.

        \param Input An input sequence
        \param N A length of the head
        \param Format A substitute sequence
        \return A modified copy of the input
    */
    template< typename InputSeqT, typename FormatSeqT >
    inline InputSeqT replace_tail_copy( 
        const InputSeqT& Input,
        unsigned int N,
        const FormatSeqT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::tail_finder(N),
            string_algo::const_formatter(Format) );
    }

    //! Replace tail algorithm
    /*!
        Replace the tail of the input with the given format sequence. 
        Tail is a suffix of a seqence of given size. 
        If the sequence is shorter then required, whole sequence if 
        considered to be the tail. Input sequence is modified in-place.

        \param Input An input sequence
        \param N A length of the head
        \param Format A substitute sequence
        \return A reference to the modified input
    */
    template< typename InputSeqT, typename FormatSeqT >
    inline InputSeqT& replace_tail( 
        InputSeqT& Input,
        unsigned int N,
        const FormatSeqT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::tail_finder(N),
            string_algo::const_formatter(Format) );
    }

} // namespace boost

#endif  // BOOST_REPLACE_HPP
