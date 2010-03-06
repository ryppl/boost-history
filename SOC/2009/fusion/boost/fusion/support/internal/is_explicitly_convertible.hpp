/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_IS_EXPLICITLY_CONVERTIBLE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_IS_EXPLICITLY_CONVERTIBLE_HPP

#include <boost/config.hpp>
#if BOOST_WORKAROUND(__GNUC__,<4)
#   include <boost/type_traits/is_convertible.hpp>
#else
#   include <boost/fusion/support/internal/small_big_type.hpp>
#   include <boost/mpl/bool.hpp>
#   include <cstddef>
#endif

namespace boost { namespace fusion { namespace detail
{
//cschmidt: Fall back due to a defect in gcc 3.x's call_expr...
#if BOOST_WORKAROUND(__GNUC__,<4)
    template<typename From, typename To>
    struct is_explicitly_convertible
      : is_convertible<From,To>
    {};
#else
    template<typename T>
    T
    get_t();

    template<std::size_t>
    struct helper;

    template<>
    struct helper<sizeof(int)>
    {
        typedef big_type type;
    };

    template<typename From, typename To>
    struct is_explicitly_convertible_impl
    {
        template<typename T>
        static typename helper<sizeof(static_cast<T>(get_t<From>()),0)>::type
        impl(int);

        template<typename>
        static small_type
        impl(...);

        typedef mpl::bool_<sizeof(impl<To>(0)) == sizeof(big_type)> type;
    };

    template<typename From, typename To>
    struct is_explicitly_convertible
      : is_explicitly_convertible_impl<From,To>::type
    {};
#endif
}}}

#endif
