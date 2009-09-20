//
// is_assoc_iter.hpp - meta function to see if an iterator is for an
//                     associative container, which we assume is true if
//                     value_type is a pair.
//
// Copyright (c) 2007 Danny Havenith
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef STREAM_IS_ASSOC_ITER_INCLUDED
#define STREAM_IS_ASSOC_ITER_INCLUDED

#include <map>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace explore
{
    namespace detail
    {
        struct false_type
        {
            char member[100];
        };

        // crude SFINAE-and-sizeof. uses the fact that a pointer to a value
        // type is probably different from than that of false_type. Cleaning
        // this up would take some more LOC.
        template< typename T>
        typename T::value_type* has_value_func(const T &);

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
        // of a map or multimap of which the iterator matches iterator_t
        //
        template< typename value_t, typename iterator_t>
        struct pair_matches_map : boost::mpl::false_ {};

        template< typename F, typename S, typename iterator_t>
        struct pair_matches_map< std::pair< F, S>, iterator_t> :
            boost::mpl::or_<
                boost::is_same<
                    iterator_t,
                    typename std::map<
                        typename boost::remove_const<F>::type, S >::iterator
                >,
                boost::is_same<
                    iterator_t,
                    typename std::multimap<
                        typename boost::remove_const<F>::type, S >::iterator
                > >
        {};

        template <typename T>
        struct value_matches_map :
            pair_matches_map< typename T::value_type, T> {};
    }

    // an associative iterator has a value type of pair<>
    template <typename T>
    struct is_assoc_iter :
        boost::mpl::eval_if<
            detail::has_value_type<T>,
            detail::value_matches_map<T>,
            boost::mpl::false_
        > {};
}}

#endif
