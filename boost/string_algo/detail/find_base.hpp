//  Boost string_algo library find_base.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_BASE_DETAIL_HPP
#define BOOST_STRING_FIND_BASE_DETAIL_HPP

#include <functional>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/traits.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  find functor base -----------------------------------------------//

            // base for find functors
            /*
                defines types and provides no-match functionality
            */
            template< typename InputT, typename InputPolicy=mutable_input_policy >
            struct find_baseF
            {
                typedef typename InputPolicy::input_policy<InputT> input_policy;

                typedef typename input_policy::input_type input_type;
                typedef typename input_policy::iterator_type input_iterator_type;
                typedef typename input_policy::reference_type input_reference_type;
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FIND_BASE_DETAIL_HPP
