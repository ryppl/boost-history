/*==============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <boost/mpl/not.hpp>
#endif
#include <boost/type_traits/is_same.hpp>
#include <boost/ref.hpp>

//TODO doc
namespace boost { namespace fusion
{
    namespace traits
    {
#if BOOST_WORKAROUND(BOOST_MSVC,==1600)
        template<typename T>
        struct deduce_stage2
        {
            typedef T type;
        };

        template<typename T>
        struct deduce
          : deduce_stage2<T>
        {};
#else
        template<typename T>
        struct deduce
        {
            typedef T type;
        };
#endif

        template<typename T>
        struct deduce<reference_wrapper<T> >
        {
            typedef T& type;
        };

#define BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(MODIFIER,DEDUCE)              \
        template<typename T>                                                    \
        struct DEDUCE<T MODIFIER>                                               \
        {                                                                       \
            typedef detail::is_po_callable<T MODIFIER> is_po_callable;          \
                                                                                \
            typedef typename                                                    \
                mpl::eval_if_c<                                                 \
                    is_po_callable::type::value                                 \
                  , mpl::if_c<                                                  \
                        is_po_callable::is_pointer::value                       \
                      , T                                                       \
                      , T MODIFIER                                              \
                    >                                                           \
                  , deduce<T>                                                   \
                >::type                                                         \
            type;                                                               \
        };                                                                      \
                                                                                \
        template<typename T>                                                    \
        struct deduce<reference_wrapper<T> MODIFIER>                            \
        {                                                                       \
            typedef T& type;                                                    \
        };

        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(volatile&,deduce)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(const volatile&,deduce)

#if BOOST_WORKAROUND(BOOST_MSVC,==1600)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(volatile&&,deduce_stage2)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(
            const volatile&&,deduce_stage2)

        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(&,deduce)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(const&,deduce)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(&&,deduce_stage2)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(const&&,deduce_stage2)
#else
#   ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(volatile&&,deduce)
        BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION(const volatile&&,deduce)
#   endif
        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(
            BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION,deduce)
#endif

#undef BOOST_FUSION_DEDUCE_CV_REF_SPECIALIZATION

#define BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(MODIFIER1,MODIFIER2,MODIFIER3)\
        template<typename T, int N>\
        struct deduce<T MODIFIER1[N]>\
        {\
            typedef T MODIFIER2(MODIFIER3 type)[N];\
        };

        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(BOOST_PP_EMPTY(),const,&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(const,const,&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(volatile,volatile,&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(
            const volatile,const volatile,&)

        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION((&),BOOST_PP_EMPTY(),&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(const(&),const,&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(volatile(&),volatile,&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(
            const volatile(&),const volatile,&)

#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION((&&),BOOST_PP_EMPTY(),&&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(const(&&),const,&&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(volatile(&&),volatile,&&)
        BOOST_FUSION_DEDUCE_ARRAY_SPECIALIZATION(
            const volatile(&&),const volatile,&&)
#endif
    }

    namespace detail
    {
        template<typename T>
        struct deduce_ref
        {
            typedef typename traits::deduce<T>::type deduced;
            typedef
                mpl::and_<
                    is_lrref<T>
                  , is_same<typename identity<T>::type,deduced>
                >
            is_regular_reference;

#ifdef BOOST_NO_RVALUE_REFERENCES
            typedef typename
                mpl::if_c<
                    is_regular_reference::value
                  , T
                  , typename add_lref<deduced>::type
                >::type
            type;
#else
            //8.5.3p5...
            typedef typename
                mpl::eval_if_c<
                    is_regular_reference::value
                  , mpl::if_c<
                        mpl::or_<
                            mpl::not_<is_rref<T> >
                          , is_class<deduced>
                          , is_array<deduced>
                        >::value
                      , T
                      , deduced&
                    >
                  , add_lref<deduced>
                >::type
            type;
#endif
        };
    }
}}

#endif
