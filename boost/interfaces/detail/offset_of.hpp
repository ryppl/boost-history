// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_OFFSET_OF_HPP_INCLUDED
#define BOOST_IDL_DETAIL_OFFSET_OF_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/is_base_and_derived.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/iter_fold_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace interfaces { namespace detail {

struct offset_of_impl {
    template<typename Subinterface, typename Superinterface>
    struct apply;

    template<typename Superinterface>
    struct not_done {
        template<typename Iter>
        struct apply {
            typedef typename mpl::deref<Iter>::type current;
            typedef typename 
                    mpl::not_<
                        mpl::or_<
                            is_same<current, Superinterface>,
                            is_base_and_derived<current, Superinterface>
                        >
                    >::type type;
        };
    };

    struct add_table_size {
        template<typename T1, typename T2>
        struct apply {
            typedef typename mpl::deref<T2>::type             base;
            typedef typename base::interface_initializer      initializer;
            typedef mpl::int_<T1::value + initializer::size>  type;
        };
    };

    template<typename Interface>
    struct apply<Interface, Interface> {
        typedef mpl::int_<0> type;
    };

    template<typename Subinterface, typename Superinterface>
    struct apply {
        typedef typename Subinterface::interface_extends        superinterfaces;
        typedef typename
                mpl::iter_fold_if<
                    superinterfaces,
                    mpl::int_<1>,
                    add_table_size,
                    not_done<Superinterface>
                >::type                                         result;
        typedef typename mpl::second<result>::type              iterator;
        typedef typename 
                mpl::deref< 
                    typename mpl::second<result>::type 
                >::type                                         value;
        typedef typename 
                mpl::plus<
                    typename mpl::first<result>::type,
                    typename 
                    offset_of_impl::apply<
                        typename 
                        mpl::deref< 
                            typename mpl::second<result>::type
                        >::type,
                        Superinterface
                    >::type
                >::type                                         type;
    };
};

template<typename Subinterface, typename Superinterface>
struct offset_of
    : offset_of_impl::template apply<Subinterface, Superinterface>::type
    { };

} } } // End namespaces detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_OFFSET_OF_HPP_INCLUDED
