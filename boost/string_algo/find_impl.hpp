//  Boost string_algo library find_impl.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_IMPL_HPP
#define BOOST_STRING_FIND_IMPL_HPP

#include <functional>
#include "traits.hpp"
#include "iterator_range.hpp"

namespace boost {

    namespace string_algo {

//  Generic find functions -------------------------------------//

        // find iterator version
        template< 
            typename InputIteratorT, 
            typename SearchIteratorT, 
            typename FindF >
        inline typename string_algo::
            iter_search_traits< InputIteratorT, SearchIteratorT>::range_type 
        find( 
            InputIteratorT Begin, 
            InputIteratorT End, 
            SearchIteratorT SearchBegin,
            SearchIteratorT SearchEnd,
            FindF Find )
        {
            // Forward the call to the functor
            return Find( 
                string_algo::make_range( Begin, End ),
                string_algo::make_range( SearchBegin, SearchEnd ) );
        }

        // find sequence const version
        template< typename InputT, typename SearchT, typename FindF >
        inline typename string_algo::search_traits<InputT, SearchT>::const_range_type 
        find( 
            const InputT& Input, 
            const SearchT& Search,
            FindF Find)
        {
            return Find( Input, Search );
        }

        // find sequence non-const version
        template< typename InputT, typename SearchT, typename FindF >
        inline typename string_algo::search_traits<InputT,SearchT>::range_type 
        BOOST_STRING_NON_CONST_FUNCTION(find)( 
            InputT& Input, 
            const SearchT& Search,
            FindF Find )
        {
            return Find( Input, Search );
        }

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_FIND_IMPL_HPP
