//  Boost string_algo library slist_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_STD_SLIST_TRAITS_HPP
#define BOOST_STRING_STD_SLIST_TRAITS_HPP

#include <boost/string_algo/yes_no_type.hpp>
#include <slist>

namespace boost {

    namespace string_algo {

//  SGI's std::slist<> traits  -----------------------------------------------//

        // stable iterators tester
        template<typename T, typename AllocT>
        yes_type sequence_has_stable_iterators( const std::slist<T,AllocT>* );

        // const time insert tester
        template<typename T, typename AllocT>
        yes_type sequence_has_const_time_insert( const std::slist<T,AllocT>* );

        // const time erase tester
        template<typename T, typename AllocT>
        yes_type sequence_has_const_time_erase( const std::slist<T,AllocT>* );

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_STD_LIST_TRAITS_HPP
