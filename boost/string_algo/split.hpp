//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_SPLIT_HPP
#define BOOST_STRING_SPLIT_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/split2.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/compare.hpp>

/*! \file
	This file contains a set of basic split algorithms. 
	Split algorithms can be used to divide a sequence
	into several part accorfing to a given criterium.
	Result is given as a 'container of containers' where
	elements are copies or references to extracted parts.
	This file contains some common specializations of generic
	algoritms contained in the file split2.hpp
*/

namespace boost {

//  find_all  ------------------------------------------------------------//

    //! Find all algorithm
	/*!
		This algorithm finds all occurences of the search sequence
		in the input. A result is given as a 'container of containers'.
		Each match of the search sequence is represented by one
		element in the result.

		\param Result A 'container container' to container the result of search.
			Both outher and inner container must have constructor taking a pair
			of iterators as an argument.
			Typical type of the result is 
				std::vector< boost::iterator_range<iterator> >.
			( each element of such a vector will container a range delimiting 
			a match )
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\return A reference the result

		\note Prior content of the result will be overriden.
	*/
    template< typename ResultT, typename InputContainerT, typename SearchContainerT >
    inline ResultT& find_all(
        ResultT& Result,
        const InputContainerT& Input,
        const SearchContainerT& Search)
    {
        return string_algo::iter_find(
            Result,
            Input,
            string_algo::first_finder(Search) );        
    }

    //! Find all algorithm ( case insensitive ) 
	/*!
		This algorithm finds all occurences of the search sequence
		in the input. A result is given as a 'container of containers'.
		Each match of the search sequence is represented by one
		element in the result. Searching is case insensitive.

		\param Result A 'container container' to container the result of search.
			Both outher and inner container must have constructor taking a pair
			of iterators as an argument.
			Typical type of the result is 
				std::vector< boost::iterator_range<iterator> >.
			( each element of such a vector will container a range delimiting 
			a match )
		\param Input A container which will be searched.
		\param Search A string to be searched for.
		\return A reference the result

		\note Prior content of the result will be overriden.
	*/
    template< typename ResultT, typename InputContainerT, typename SearchContainerT >
    inline ResultT& ifind_all(
        ResultT& Result,
        const InputContainerT& Input,
        const SearchContainerT& Search)
    {
        return string_algo::iter_find(
            Result,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal() ) );        
    }


//  tokenize  -------------------------------------------------------------//

    //! Split algorithm
	/*! 
		Tokenize expression. This function is equivalent of C strtok. Input
		sequence is split into tokens, separated by seperators. Separators 
		are given in the mean of predicate.

		\param Result A 'container container' to container the result of search.
			Both outher and inner container must have constructor taking a pair
			of iterators as an argument.
			Typical type of the result is 
				std::vector< boost::iterator_range<iterator> >.
			( each element of such a vector will container a range delimiting 
			a match )

		\param Input A container which will be searched.
		\param Pred A predcicate to indentify separators. This predicate is 
			supposed to return true if a given element is a separator.
		\param bCompress If bCompress argument is set to true, adjancent 
			separators are merged together. Otherwise, every two separators
			delimit a token.
		\return A reference the result

		\note Prior content of the result will be overriden.
    */
    template< typename ResultT, typename InputContainerT, typename PredicateT >
    inline ResultT& split(
        ResultT& Result,
        const InputContainerT& Input,
        PredicateT Pred,
        bool bCompress=true )
    {
        return string_algo::iter_split(
            Result,
            Input,
            string_algo::token_finder( Pred, bCompress ) );         
    }

} // namespace boost


#endif  // BOOST_STRING_SPLIT_HPP
