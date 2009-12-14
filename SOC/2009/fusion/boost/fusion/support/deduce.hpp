/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_HPP

#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/ref.hpp>

//TODO doc
namespace boost { namespace fusion
{
    namespace traits
    {
        template <typename T>
        struct deduce
        {
            typedef T type;
        };

        template <typename T>
        struct deduce<reference_wrapper<T> >
        {
            typedef T& type;
        };

#define BOOST_FUSION_CV_REF_SPECIALIZATION(MODIFIER,_)\
        template <typename T>\
        struct deduce<T MODIFIER>\
        {\
            typedef detail::is_po_callable<T MODIFIER> is_po_callable;\
            \
            typedef typename\
                mpl::eval_if<\
                    typename is_po_callable::type\
                  , mpl::if_<\
                        typename is_po_callable::is_pointer\
                      , T\
                      , T MODIFIER\
                    >\
                  , deduce<T>\
                >::type\
            type;\
        };\
        \
        template <typename T>\
        struct deduce<reference_wrapper<T> MODIFIER>\
        {\
            typedef T& type;\
        };

        BOOST_FUSION_CV_REF_SPECIALIZATION(volatile&,_)
        BOOST_FUSION_CV_REF_SPECIALIZATION(const volatile&,_)
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_FUSION_CV_REF_SPECIALIZATION(volatile&&,_)
        BOOST_FUSION_CV_REF_SPECIALIZATION(const volatile&&,_)
#endif
        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(BOOST_FUSION_CV_REF_SPECIALIZATION,_)

#undef BOOST_FUSION_CV_REF_SPECIALIZATION

        template <typename T, int N>
        struct deduce<T[N]>
        {
            typedef const T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<const T[N]>
        {
            typedef const T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<volatile T[N]>
        {
            typedef const volatile T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<const volatile T[N]>
        {
            typedef const volatile T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<T(&)[N]>
        {
            typedef T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<const T(&)[N]>
        {
            typedef const T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<volatile T(&)[N]>
        {
            typedef volatile T(&type)[N];
        };

        template <typename T, int N>
        struct deduce<const volatile T(&)[N]>
        {
            typedef const volatile T(&type)[N];
        };
    }

    namespace detail
    {
        template <typename T>
        struct deduce_ref
        {
            typedef typename traits::deduce<T>::type deduced;

            typedef typename
                mpl::if_<
                    mpl::and_<
                        detail::is_lrref<T>
                      , is_same<typename identity<T>::type,deduced>
                    >
                  , T
                  , typename add_lref<deduced>::type
                >::type
            type;
        };
    }
}}

#endif
