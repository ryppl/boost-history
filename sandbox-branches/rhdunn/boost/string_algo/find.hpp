//  Boost string_algo library find.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/compare.hpp>

/*! \file
    Defines a set of find algorithms. The algoritms are searching
    for a subsequence of the input. The result is given as an \c iterator_range
    delimiting the substring.
*/

namespace boost {

//  find_first  -----------------------------------------------//

    //! Find first algorithm
    /*!
        Search for a first match of search sequence in the 
        input container. 
        
        \param Input A container which will be searched.
        \param Search A string to be searched for.
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find_first( 
        InputContainerT& Input, 
        const SearchContainerT& Search )
    {
        return string_algo::first_finder(Search)(
            string_algo::begin(Input),string_algo::end(Input));
    }

    //! Find first algorithm ( case insensitive )
    /*!
        Search for a first match of search sequence in the 
        input container. Searching is case insensitive.
        
        \param Input A container which will be searched.
        \param Search A string to be searched for.
        \param Loc a locale used for case insensitive comparison
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    ifind_first( 
        InputContainerT& Input, 
        const SearchContainerT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::first_finder(Search,string_algo::is_iequal(Loc))(
            string_algo::begin(Input),string_algo::end(Input));
    }

//  find_last  -----------------------------------------------//

    //! Find last algorithm
    /*!
        Search for a last match of search sequence in the 
        input container. 
        
        \param Input A container which will be searched.
        \param Search A string to be searched for.
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find_last( 
        InputContainerT& Input, 
        const SearchContainerT& Search )
    {
        return string_algo::last_finder(Search)(
            string_algo::begin(Input),string_algo::end(Input));
    }

    //! Find last algorithm ( case insensitive )
    /*!
        Search for a last match of search sequence in the 
        input container. Searching is case insensitive.
        
        \param Input A container which will be searched.
        \param Search A string to be searched for.
        \param Loc a locale used for case insensitive comparison
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    ifind_last( 
        InputContainerT& Input, 
        const SearchContainerT& Search,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::last_finder(Search, string_algo::is_iequal(Loc))(
            string_algo::begin(Input),string_algo::end(Input));
    }

//  find_nth ----------------------------------------------------------------------//

    //! Find n-th algorithm 
    /*!
        Search for an n-th match of search sequence in the 
        input sequence.         
        
        \param Input A container which will be searched.
        \param Search A string to be searched for.
        \param Nth An index of the match to be found.
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find_nth( 
        InputContainerT& Input, 
        const SearchContainerT& Search,
        unsigned int Nth )
    {
        return string_algo::nth_finder(Search,Nth)(
            string_algo::begin(Input),string_algo::end(Input));
    }

    //! Find n-th algorithm ( case insensitive ).
    /*!
        Search for an n-th match of search sequence in the 
        input sequence. Searching is case insensitive.
        
        \param Input A container which will be searched.
        \param Search A string to be searched for.
        \param Nth An index of the match to be found.
        \param Loc a locale used for case insensitive comparison
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    ifind_nth( 
        InputContainerT& Input, 
        const SearchContainerT& Search,
        unsigned int Nth,
        const std::locale& Loc=std::locale() )
    {
        return string_algo::nth_finder(Search,Nth,string_algo::is_iequal(Loc))(
            string_algo::begin(Input),string_algo::end(Input));
    }

//  find_head ----------------------------------------------------------------------//

    //! Find head algorithm
    /*!
        Get the head of the input. Head is a prefix of 
        a seqence of given size. If the sequence is shorter then required,
        whole sequence if considered to be the head.

        \param Input A container which will be searched.
        \param N A length of the head
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find_head( 
        InputContainerT& Input, 
        unsigned int N )
    {
        return string_algo::head_finder(N)(
            string_algo::begin(Input),string_algo::end(Input));      
    }

//  find_tail ----------------------------------------------------------------------//

    //! Find tail algorithm
    /*!
        Get the tail of the input. Head is a suffix of 
        a seqence of given size. If the sequence is shorter then required,
        whole sequence if considered to be the tail.

        \param Input A container which will be searched.
        \param N A length of the tail
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find_tail( 
        InputContainerT& Input, 
        unsigned int N )
    {
        return string_algo::tail_finder(N)(
            string_algo::begin(Input),string_algo::end(Input));      
    }

//  find_token --------------------------------------------------------------------//

    //! Find token algorithm
    /*!
        Look for a given token in the sequence. Token is specified by a predicate.
        If bCompressed is given, adjancent tokens are considered to be one match.
        
        \param Input A container which will be searched.
        \param Pred An unary predicate to identify a tiken
        \param bCompress Enable/Disable compressing of adjancent tokens
        \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c InputContainerT::iterator or 
            \c InputContainerT::const_iterator, depending on the constness of 
            the input parameter.
    */
    template< typename InputContainerT, typename PredicateT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find_token( 
        InputContainerT& Input,
        PredicateT Pred,
        bool bCompress=true )
    {
        return string_algo::token_finder(Pred, bCompress)(
            string_algo::begin(Input),string_algo::end(Input));       
    }


} // namespace boost


#endif  // BOOST_STRING_FIND_HPP
