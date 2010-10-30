/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_BASE_REF_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_BASE_REF_HPP

#include <boost/preprocessor/empty.hpp>
#include <boost/mpl/if.hpp>
#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
#   include <boost/mpl/eval_if.hpp>
#   include <boost/mpl/or.hpp>
#   include <boost/mpl/identity.hpp>
#endif
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#if BOOST_WORKAROUND(BOOST_MSVC,==1600)
#   include <boost/type_traits/is_function.hpp>
#endif
#ifdef BOOST_FUSION_PROPAGATE_VOLATILE
#   include <boost/type_traits/is_volatile.hpp>
#   include <boost/type_traits/add_volatile.hpp>
#   include <boost/type_traits/remove_cv.hpp>
#else
#   include <boost/type_traits/remove_const.hpp>
#endif

#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
#   include <utility>
#endif

//cschmidt: We ignore volatile in the BOOST_FUSION_ALL_CV_*-Macros, as we would
//get a lot of problems with older compilers. On top of that, fusion
//types are not meant to be volatile anyway.
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
#   define BOOST_FUSION_RREF_ELSE_OBJ(OBJECT) OBJECT
#   define BOOST_FUSION_R_ELSE_LREF(OBJECT) OBJECT&
#   define BOOST_FUSION_R_ELSE_CLREF(OBJECT) OBJECT const&

#   define BOOST_FUSION_FORWARD(TYPE,ARGUMENT) ARGUMENT

#   define BOOST_FUSION_ALL_CTOR_COMBINATIONS(MACRO,ARG)\
        MACRO(const&,ARG)
#   define BOOST_FUSION_ALL_CV_REF_COMBINATIONS(MACRO,ARG)\
        MACRO(const&,ARG)\
        MACRO(&,ARG)
#else
#   define BOOST_FUSION_RREF_ELSE_OBJ(OBJECT) OBJECT&&
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
#if BOOST_WORKAROUND(BOOST_MSVC, == 1600)
    template<typename T>
    struct is_lrref_stage2
      : mpl::false_
    {};

    template<typename T>
    struct is_lrref_stage2<T&&>
      : mpl::true_
    {};

    template<typename T>
    struct is_lrref
      : is_lrref_stage2<T>
    {};
#else
    template<typename T>
    struct is_lrref
      : mpl::false_
    {};

#   ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct is_lrref<T&&>
      : mpl::true_
    {};
#   endif
#endif

    template<typename T>
    struct is_lrref<T&>
      : mpl::true_
    {};

#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct is_rref
      : mpl::false_
    {};

    template<typename T>
    struct is_rref<T&&>
      : mpl::true_
    {};
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, == 1600)
    template<typename T>
    struct remove_reference_stage2
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference_stage2<T&&>
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference
      : remove_reference_stage2<T>
    {};
#else
    template<typename T>
    struct remove_reference
    {
        typedef T type;
    };

#   ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct remove_reference<T&&>
    {
        typedef T type;
    };
#   endif
#endif

    template<typename T>
    struct remove_reference<T&>
    {
        typedef T type;
    };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1600)
    template<typename T>
    struct add_lref_stage2
    {
        typedef T& type;
    };

    template<typename T>
    struct add_lref_stage2<T&&>
    {
        typedef T&& type;
    };

    template<typename T>
    struct add_lref
      : add_lref_stage2<T>
    {};
#else
    template<typename T>
    struct add_lref
    {
        typedef T& type;
    };

#   ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct add_lref<T&&>
    {
        typedef T&& type;
    };
#   endif
#endif

    template<typename T>
    struct add_lref<T&>
    {
        typedef T& type;
    };

    template<typename T>
    struct identity
      :
#ifndef BOOST_FUSION_PROPAGATE_VOLATILE
        remove_const<
#else
        remove_cv<
#endif
            typename remove_reference<T>::type
        >
    {};

    template<typename TestType,typename Type>
    struct forward_as
    {
        typedef typename remove_reference<TestType>::type test_type;

        typedef typename
            mpl::if_c<
                is_const<test_type>::value
              , typename add_const<Type>::type
              , Type
            >::type
#ifdef BOOST_FUSION_PROPAGATE_VOLATILE
        const_type;

        typedef typename
            mpl::if_c<
                is_volatile<test_type>::value
              , typename add_volatile<const_type>::type
              , const_type
            >::type
#endif
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
        &type;
#else
        cv_type;

        typedef typename
            mpl::eval_if_c<
                //8.5.3p5...
                is_rref<TestType>::value
              , mpl::if_c<
                    mpl::or_<is_class<Type>, is_array<Type> >::value
                  , cv_type&&
                  , cv_type
                >
              , mpl::identity<cv_type&>
            >::type
        type;
#endif
    };

#if !defined(BOOST_MSVC) || BOOST_MSVC!=1600
    template<typename TestType,typename Type>
    struct forward_as<TestType,Type&>
    {
        typedef Type& type;
    };
#endif

#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    //8.5.3p5...
    template<typename TestType,typename Type>
    struct forward_as<TestType,Type&&>
      : mpl::if_c<
            mpl::or_<is_class<Type>, is_array<Type> >::value
          , Type&&
#if BOOST_WORKAROUND(BOOST_MSVC,==1600)
          , typename mpl::if_c<
                is_function<Type>::value
              , Type&
              , Type
            >::type
#else
          , Type
#endif
        >
    {};
#endif

    template<typename TestType, typename Type>
    struct forward_as_lref
      : forward_as<TestType, Type>
    {};

#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename TestType,typename Type>
    struct forward_as_lref<TestType,Type&&>
      : forward_as<TestType, Type&>
    {};
#endif
}}}

#endif
