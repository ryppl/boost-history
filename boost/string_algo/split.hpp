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
#include <boost/string_algo/find_iterator.hpp>
#include <boost/string_algo/find_impl.hpp>


namespace boost {

//  find_all  ------------------------------------------------------------//

    // find all the matches of a subsequnce in a sequence
    template< typename InputT, typename SearchT >
    inline iterator_range< 
	string_algo::detail::find_iterator<
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator,
		string_algo::detail::first_finderF<
			BOOST_STRING_TYPENAME 
				string_algo::container_traits<SearchT>::const_iterator> > >
    find_all( 
        InputT& Input, 
        const SearchT& Search )
    {
		return string_algo::iter_find( 
			Input,
			string_algo::first_finder(Search) ); 		
    }

	// copy version of find_all
	template< typename ResultT, typename InputT, typename SearchT >
	inline ResultT& find_all_copy(
		ResultT& Result,
		const InputT& Input,
		const SearchT& Search)
	{
		return string_algo::iter_find_copy(
			Result,
			Input,
			string_algo::first_finder(Search) ); 		
	}

//  tokenize  -------------------------------------------------------------//

	// tokenize expression ( split )
	/* 
		This function is equivalent of C strtok. Separators a given
		in the mean of predicate.
		If bCompress argument is set to true, adjancent separators
		are merged together.
	*/
    template< typename InputT, typename PredicateT >
    inline iterator_range< 
	string_algo::detail::find_iterator<
        BOOST_STRING_TYPENAME string_algo::container_traits<InputT>::result_iterator,
		string_algo::detail::token_finderF<PredicateT>,
		string_algo::detail::split_find_policy> >
    split( 
        InputT& Input, 
		PredicateT Pred,
		bool bCompress=true )
	{
		return string_algo::iter_split( 
			Input,
			string_algo::token_finder( Pred, bCompress ) ); 		
    }

	// copy version of split
	template< typename ResultT, typename InputT, typename PredicateT >
	inline ResultT& split_copy(
		ResultT& Result,
		const InputT& Input,
		PredicateT Pred,
		bool bCompress=true )
	{
		return string_algo::iter_split_copy(
			Result,
			Input,
			string_algo::token_finder( Pred, bCompress ) ); 		
	}

} // namespace boost


#endif  // BOOST_STRING_SPLIT_HPP
