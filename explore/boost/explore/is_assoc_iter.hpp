//
// is_assoc_iter.hpp - meta function to see if an iterator is for an associative
//                     container, which we assume is true if value_type is a pair.
//
// Copyright (c) 2007 Danny Havenith
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

namespace explore
{
    namespace detail
    {
        struct false_type
        {
            char member[100];
        };

        // crude SFINAE-and-sizeof. uses the fact that a pointer to a value type
        // is probably different from than that of false_type. Cleaning this up would take
        // some more LOC.
        template< typename T>
        typename T::value_type* has_value_func( const T &);

        false_type has_value_func(...);

        template <typename T>
        T& make_t();

        template< typename T>
        struct has_value_type : boost::mpl::bool_<
            sizeof(false_type)
            !=
            sizeof(has_value_func(make_t<T>()))>
        {};

        //
        // returns true if the value_t could be the value_type
        // of a map of which the iterator matches iterator_t
        //
        template<typename T>
        struct is_pair : boost::mpl::false_ {};

        template<typename T1, typename T2>
        struct is_pair<std::pair<T1, T2> > : boost::mpl::true_ {};

        template<typename T>
        struct value_type_is_pair : is_pair<typename T::value_type> {};
    }

    // an associative iterator has a value type of pair<>
    template <typename T>
    struct is_assoc_iter :
        boost::mpl::eval_if<
            detail::has_value_type<T>,
            detail::value_type_is_pair<T>,
            boost::mpl::false_
        > {};
}
