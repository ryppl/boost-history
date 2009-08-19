/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_AS_FUSION_ELEMENT_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_AS_FUSION_ELEMENT_HPP

//TODO cschmidt: rref

#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/ref.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename T>
    struct as_fusion_element
    {
        typedef T type;
    };

    template <typename T>
    struct as_fusion_element<reference_wrapper<T> >
    {
        typedef T& type;
    };

    //TODO preserve rvalues?
#define BOOST_FUSION_CV_REF_SPECIALIZATION(COMBINATION,_)\
    template <typename T>\
    struct as_fusion_element<T COMBINATION>\
    {\
        typedef typename\
            mpl::eval_if<\
                detail::is_po_callable<T COMBINATION>\
              , mpl::identity<T COMBINATION>\
              , as_fusion_element<T>\
            >::type\
        type;\
    };\
    \
    template <typename T>\
    struct as_fusion_element<reference_wrapper<T> COMBINATION>\
    {\
        typedef T& type;\
    };

    BOOST_FUSION_CV_REF_SPECIALIZATION(volatile&,_)
    BOOST_FUSION_CV_REF_SPECIALIZATION(const volatile&,_)
    BOOST_FUSION_ALL_CV_REF_COMBINATIONS(BOOST_FUSION_CV_REF_SPECIALIZATION,_)

#undef BOOST_FUSION_CV_REF_SPECIALIZATION

    template <typename T, int N>
    struct as_fusion_element<T(&)[N]>
    {
        typedef T(&type)[N];
    };

    template <typename T, int N>
    struct as_fusion_element<const T(&)[N]>
    {
        typedef const T(&type)[N];
    };

    template <typename T, int N>
    struct as_fusion_element<volatile T(&)[N]>
    {
        typedef volatile T(&type)[N];
    };

    template <typename T, int N>
    struct as_fusion_element<const volatile T(&)[N]>
    {
        typedef const volatile T(&type)[N];
    };

    template <typename T>
    struct as_fusion_element_ref
    {
        typedef typename as_fusion_element<T>::type element;

        typedef typename
            mpl::if_<
                mpl::and_<
                    is_lrref<T>
                  , is_same<typename identity<T>::type,element>
                >
              , T
              , typename add_lref<element>::type
            >::type
        type;
    };
}}}

#endif
