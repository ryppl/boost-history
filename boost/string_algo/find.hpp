//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/compare.hpp>

namespace boost {

//  find_first  -----------------------------------------------//

    //! Find first algorithm
    /*!
        Search for a first match of search sequence in the 
        input container. 
		
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of 
			the input parameter.
	*/
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    find_first( 
        InputContainerT& Input, 
        const SearchContainerT& Search )
    {
        return string_algo::first_finder(Search)(Input);
    }

    //! Find first algorithm ( case insensitive )
    /*!
        Search for a first match of search sequence in the 
        input container. Searching is case insensitive.
		
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    ifind_first( 
        InputContainerT& Input, 
        const SearchContainerT& Search )
    {
        return string_algo::first_finder(Search,string_algo::is_iequal())(Input);
    }

//  find_last  -----------------------------------------------//

    //! Find last algorithm
    /*!
        Search for a last match of search sequence in the 
        input container. 
		
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    find_last( 
        InputContainerT& Input, 
        const SearchContainerT& Search )
    {
        return string_algo::last_finder(Search)(Input);
    }

    //! Find last algorithm ( case insensitive )
    /*!
        Search for a last match of search sequence in the 
        input container. Searching is case insensitive.
		
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of the 
			input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    ifind_last( 
        InputContainerT& Input, 
        const SearchContainerT& Search )
    {
        return string_algo::last_finder(Search, string_algo::is_iequal())(Input);
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
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of 
			the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    find_nth( 
        InputContainerT& Input, 
        const SearchContainerT& Search,
        unsigned int Nth )
    {
        return string_algo::nth_finder(Search,Nth)(Input);
    }

    //! Find n-th algorithm ( case insensitive ).
    /*!
        Search for an n-th match of search sequence in the 
        input sequence. Searching is case insensitive.
		
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\param Nth An index of the match to be found.
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness 
			of the input parameter.
    */
    template< typename InputContainerT, typename SearchContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    ifind_nth( 
        InputContainerT& Input, 
        const SearchContainerT& Search,
        unsigned int Nth )
    {
        return string_algo::nth_finder(Search,Nth,string_algo::is_iequal())(Input);
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
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness 
			of the input parameter.
    */
    template< typename InputContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    find_head( 
        InputContainerT& Input, 
        unsigned int N )
    {
        return string_algo::head_finder( N )( Input );      
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
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness 
			of the input parameter.
    */
    template< typename InputContainerT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    find_tail( 
        InputContainerT& Input, 
        unsigned int N )
    {
        return string_algo::tail_finder( N )( Input );      
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
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator 
			or InputContainerT::const_iterator, depending on the constness 
			of the input parameter.
    */
    template< typename InputContainerT, typename PredicateT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_traits<InputContainerT>::result_iterator >
    find_token( 
        InputContainerT& Input,
        PredicateT Pred,
        bool bCompress=true )
    {
        return string_algo::token_finder( Pred, bCompress )( Input );       
    }


} // namespace boost


#endif  // BOOST_STRING_FIND_HPP
