//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_DETAIL_HPP
#define BOOST_STRING_REPLACE_DETAIL_HPP

#include <functional>

#include "../traits.hpp"

namespace boost {

//  generic replace functors -----------------------------------------------//

    namespace string_algo {
        
        namespace detail {

//  replace identity format functor -----------------------------------------//

            template< typename FormatT >
            struct identity_formatF
            {
                typedef FormatT format_type;
                typedef const format_type& format_reference_type;

                typedef FormatT result_type;
                typedef const result_type& result_reference_type;

                template< typename ReplaceT >
                result_reference_type operator()( format_reference_type Format, ReplaceT )
                {
                    return Format;
                }

				template< typename ReplaceIteratorT >
				result_reference_type operator()( 
					format_reference_type Format, 
					ReplaceIteratorT,
					ReplaceIteratorT )
                {
                    return Format;
                }
            };

//  null format functor ( used by erase )--------------------------------------//

			template< typename ReplaceT >
			struct null_formatF
			{
                typedef unsigned int format_type;

                typedef ReplaceT result_type;
                typedef const result_type& result_reference_type;

                result_type operator()( format_type, ReplaceT )
                {
                    return ReplaceT();
                }

                result_type operator()( 
					format_type, 
					typename ReplaceT::const_iterator,
					typename ReplaceT::const_iterator )
                {
                    return ReplaceT();
                }
			};

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_REPLACE_DETAIL_HPP
