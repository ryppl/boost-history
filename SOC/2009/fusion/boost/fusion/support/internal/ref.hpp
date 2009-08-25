/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_REF_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_REF_HPP

#include <boost/config.hpp>

#include <boost/preprocessor/empty.hpp>
#include <boost/mpl/eval_if.hpp>
#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <boost/mpl/or.hpp>
#endif
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_volatile.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_cv.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <utility>
#endif

//cschmidt: We ignore volatile in the BOOST_FUSION_ALL_CV_*-Macros, as we would
//get a lot of problems with older compilers. On top of that, fusion
//types are not meant to be volatile anyway.
#ifdef BOOST_NO_RVALUE_REFERENCES
#   define BOOST_FUSION_R_ELSE_LREF(OBJECT) OBJECT&
#   define BOOST_FUSION_R_ELSE_CLREF(OBJECT) OBJECT const&

#   define BOOST_FUSION_FORWARD(TYPE,ARGUMENT) ARGUMENT

#   define BOOST_FUSION_ALL_CTOR_COMBINATIONS(MACRO,ARG)\
        MACRO(const&,ARG)
#   define BOOST_FUSION_ALL_CV_REF_COMBINATIONS(MACRO,ARG)\
        MACRO(const&,ARG)\
        MACRO(&,ARG)
#else
#   define BOOST_FUSION_R_ELSE_LREF(OBJECT) OBJECT&&
#   define BOOST_FUSION_R_ELSE_CLREF(OBJECT) OBJECT&&

#   define BOOST_FUSION_FORWARD(TYPE,ARGUMENT) std::forward<TYPE>(ARGUMENT)

#   define BOOST_FUSION_ALL_CTOR_COMBINATIONS(MACRO,ARG)\
        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(MACRO,ARG)
#   define BOOST_FUSION_ALL_CV_REF_COMBINATIONS(MACRO,ARG)\
        MACRO(&,ARG)\
        MACRO(const&,ARG)\
        MACRO(&&,ARG)\
        MACRO(const&&,ARG)
#endif
#define BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(MACRO,ARG)\
    BOOST_FUSION_ALL_CV_REF_COMBINATIONS(MACRO,ARG)\
    MACRO(BOOST_PP_EMPTY(),ARG)\
    MACRO(const,ARG)

namespace boost { namespace fusion { namespace detail
{
    //cschmidt: workaround until boost::is_reference supports rvalues
    template<typename T>
    struct is_lrref
      : mpl::false_
    {
    };

    template<typename T>
    struct is_lrref<T&>
      : mpl::true_
    {
    };

#ifndef BOOST_NO_RVALUE_REFERENCES
    template<typename T>
    struct is_lrref<T&&>
      : mpl::true_
    {
    };

    template<typename T>
    struct is_rref
      : mpl::false_
    {
    };

    template<typename T>
    struct is_rref<T&&>
      : mpl::true_
    {
    };
#endif

    //cschmidt: workaround until boost::is_reference supports rvalues
    template<typename T>
    struct remove_reference
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T&>
    {
        typedef T type;
    };

#ifndef BOOST_NO_RVALUE_REFERENCES
    template<typename T>
    struct remove_reference<T&&>
    {
        typedef T type;
    };
#endif

    template <typename T>
    struct add_lref
      : add_reference<T>
    {};

#ifndef BOOST_NO_RVALUE_REFERENCES
    template <typename T>
    struct add_lref<T&&>
    {
        typedef T&& type;
    };
#endif

    template <typename T>
    struct identity
    {
        typedef typename remove_const<
            typename remove_cv<
                typename remove_reference<T>::type
            >::type
        >::type type;
    };

    template <typename TestType,typename Type>
    struct forward_as
    {
        typedef typename remove_reference<TestType>::type test_type;

        typedef typename
            mpl::if_<
                is_const<test_type>
              , typename add_const<Type>::type
              , Type
            >::type
        const_type;

        typedef typename
            mpl::if_<
                is_volatile<test_type>
              , typename add_volatile<const_type>::type
              , const_type
            >::type
        cv_type;

#ifdef BOOST_NO_RVALUE_REFERENCES
        typedef typename add_reference<cv_type>::type type;
#else
        typedef typename
            mpl::eval_if<
                //8.5.3p5...
                mpl::or_<is_class<Type>
                  , is_array<Type>
                    //cschmidt: workaround until Boost.type_traits supports
                    //rvalue refs!
                  , mpl::not_<is_rref<TestType> >
                >,
                mpl::eval_if<
                    is_rref<TestType>
                  , mpl::identity<cv_type&&>
                  , add_lref<cv_type>
                >,
                mpl::identity<cv_type>
            >::type
        type;
#endif
    };
}}}

#endif
