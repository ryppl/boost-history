//  Boost string_algo library find_impl.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_IMPL_HPP
#define BOOST_STRING_FIND_IMPL_HPP

#include <functional>
#include <boost/string_algo/iterator_range.hpp>

namespace boost {

    namespace string_algo {

//  Generic find functions -------------------------------------//

        // find iterator version
        template< 
            typename InputIteratorT, 
            typename FindF >
        inline typename FindF::result_type 
        find( 
            InputIteratorT Begin, 
            InputIteratorT End, 
            const FindF& Find )
        {
            // Forward the call to the functor
            return Find( make_range( Begin, End ) );
        }

        // find sequence const version
        template< typename InputT, typename FindF >
        inline typename FindF::result_type 
        find( 
            const InputT& Input, 
            const FindF& Find )
        {
            return Find( Input );
        }

        // find sequence non-const version
        template< typename InputT, typename FindF >
        inline typename FindF::result_type 
        BOOST_STRING_NON_CONST_FUNCTION(find)( 
            InputT& Input, 
            const FindF& Find )
        {
            return Find( Input );
        }

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_FIND_IMPL_HPP
