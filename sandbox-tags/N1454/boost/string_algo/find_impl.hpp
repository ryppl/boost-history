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

        // find sequence version
        template< 
            typename ForwardIteratorT,
            typename FindF >
        inline typename FindF::result_type 
        find( 
            ForwardIteratorT Begin, 
            ForwardIteratorT End, 
            FindF Find )
        {
            return Find( Begin, End );
        }

        template< 
            typename InputT,
            typename FindF >
        inline typename FindF::result_type 
        find( 
            InputT& Input, 
            FindF Find )
        {
            return Find( Input.begin(), Input.end() );
        }

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_FIND_IMPL_HPP
