//  Boost string_algo library yes_no_type.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_YES_NO_TYPE_DETAIL_HPP
#define BOOST_STRING_YES_NO_TYPE_DETAIL_HPP

namespace boost {

    namespace string_algo {

        namespace detail {

            // taken from boost mailing-list
            // when yes_no_type will become officialy
            // a part of boost distribution, this header
            // will be deprecated
            template<int I> struct size_descriptor 
            {
                typedef char (& type)[I];
            }; 

            typedef size_descriptor<1>::type yes_type;
            typedef size_descriptor<2>::type no_type;

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_YES_NO_TYPE_DETAIL_HPP
