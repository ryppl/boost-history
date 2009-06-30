/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DETAIL_AS_FUSION_ELEMENT_HPP
#define BOOST_FUSION_SUPPORT_DETAIL_AS_FUSION_ELEMENT_HPP

//TODO cschmidt: rref

#include <boost/ref.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename T>
    struct as_fusion_element
    {
        typedef T type;
    };

    template <typename T>
    struct as_fusion_element<T&>
        : as_fusion_element<T>
    {
    };

    template <typename T>
    struct as_fusion_element<const T&>
        : as_fusion_element<T>
    {
    };

#ifndef BOOST_NO_RVALUE_REFERENCES
    template <typename T>
    struct as_fusion_element<T&&>
        : as_fusion_element<T>
    {
    };

    template <typename T>
    struct as_fusion_element<const T&&>
        : as_fusion_element<T>
    {
    };
#endif

    template <typename T>
    struct as_fusion_element<reference_wrapper<T> >
    {
        typedef T& type;
    };

    template <typename T>
    struct as_fusion_element<const reference_wrapper<T> >
    {
        typedef T& type;
    };

    template <typename T, int N>
    struct as_fusion_element<T[N]>
    {
        typedef const T(&type)[N];
    };

    template <typename T, int N>
    struct as_fusion_element<volatile T[N]>
    {
        typedef const volatile T(&type)[N];
    };

    template <typename T, int N>
    struct as_fusion_element<const volatile T[N]>
    {
        typedef const volatile T(&type)[N];
    };

    template <typename T>
    struct as_fusion_element_lref
    {
        typedef typename
            add_lref<typename as_fusion_element<T>::type>::type
        type;
    };

    template <typename T>
    struct as_fusion_element_lref<const T&>
      : as_fusion_element_lref<const T>
    {
    };

    template <typename T>
    struct as_fusion_element_lref<const T&&>
      : as_fusion_element_lref<const T>
    {
    };
}}}

#endif
