//  Boost string_algo library util.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_UTIL_DETAIL_HPP
#define BOOST_STRING_UTIL_DETAIL_HPP

#include <functional>

namespace boost {

    namespace string_algo {

        namespace detail {

//  equal_to functor  -----------------------------------------------//

            // equal_to functor reimplementation
            /*
                standard STL implementation does handle only comparison between types
                of the same value. This is less restrictive version which wraps == operator.
            */
            template< typename Type1, typename Type2 >
            struct equal_toF : public std::binary_function< Type1, Type2, bool > 
            {
                typedef typename std::binary_function< Type1, Type2, bool >::result_type result_type;
                typedef typename std::binary_function< Type1, Type2, bool >::first_argument_type first_argument_type;
                typedef typename std::binary_function< Type1, Type2, bool >::second_argument_type second_argument_type;

                result_type operator ()( const first_argument_type& Arg1, const second_argument_type& Arg2 ) const
                {
                    return Arg1==Arg2;
                }
            };

		} // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_UTIL_DETAIL_HPP
