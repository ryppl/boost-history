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
        template< typename FindF >
        inline typename FindF::result_type 
        find( 
            typename FindF::input_iterator_type Begin, 
            typename FindF::input_iterator_type End, 
            const FindF& Find )
        {
            // Forward the call to the functor
            return Find( make_range( Begin, End ) );
        }

        // find sequence version
        template< typename FindF >
        inline typename FindF::result_type 
        find( 
            typename FindF::input_reference_type Input, 
            const FindF& Find )
        {
            return Find( Input );
        }

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_FIND_IMPL_HPP
