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

namespace boost {

//  find_all  ------------------------------------------------------------//

    // find all the matches of a subsequnce in a sequence
    template< typename ResultT, typename InputT, typename SearchT >
    inline ResultT& find_all(
        ResultT& Result,
        const InputT& Input,
        const SearchT& Search)
    {
        return string_algo::iter_find(
            Result,
            Input,
            string_algo::first_finder(Search) );        
    }

    // find all ( case insensitive version )
    template< typename ResultT, typename InputT, typename SearchT >
    inline ResultT& ifind_all(
        ResultT& Result,
        const InputT& Input,
        const SearchT& Search)
    {
        return string_algo::iter_find(
            Result,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal() ) );        
    }


//  tokenize  -------------------------------------------------------------//

    // tokenize expression ( split )
    /* 
        This function is equivalent of C strtok. Separators a given
        in the mean of predicate.
        If bCompress argument is set to true, adjancent separators
        are merged together.
    */
    template< typename ResultT, typename InputT, typename PredicateT >
    inline ResultT& split(
        ResultT& Result,
        const InputT& Input,
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
