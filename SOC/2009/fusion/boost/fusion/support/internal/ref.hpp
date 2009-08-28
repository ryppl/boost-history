/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_REF_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_REF_HPP

#include <boost/config.hpp>

#include <boost/preprocessor/empty.hpp>
#include <boost/mpl/if.hpp>
#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <boost/mpl/eval_if.hpp>
#   include <boost/mpl/or.hpp>
#   include <boost/mpl/identity.hpp>
#endif
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#ifdef BOOST_FUSION_PROPAGATE_VOLATILE
#   include <boost/type_traits/is_volatile.hpp>
#   include <boost/type_traits/add_volatile.hpp>
#   include <boost/type_traits/remove_cv.hpp>
#else
#   include <boost/type_traits/remove_const.hpp>
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
    {};

    template<typename T>
    struct is_lrref<T&>
      : mpl::true_
    {};

#ifndef BOOST_NO_RVALUE_REFERENCES
    template<typename T>
    struct is_lrref<T&&>
      : mpl::true_
    {};

    template<typename T>
    struct is_rref
      : mpl::false_
    {};

    template<typename T>
    struct is_rref<T&&>
      : mpl::true_
    {};
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
    {
        typedef T& type;
    };

    template <typename T>
    struct add_lref<T&>
    {
        typedef T& type;
    };

    template <typename T>
    struct identity
    {
        typedef typename
#ifdef BOOST_FUSION_PROPAGATE_VOLATILE
            remove_cv<
#else
            remove_const<
#endif
                typename remove_reference<T>::type
            >::type
        type;
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
#ifdef BOOST_FUSION_PROPAGATE_VOLATILE
        const_type;

        typedef typename
            mpl::if_<
                is_volatile<test_type>
              , typename add_volatile<const_type>::type
              , const_type
            >::type
#endif
        cv_type;

#ifdef BOOST_NO_RVALUE_REFERENCES
        typedef cv_type& type;
#else
        typedef typename
            mpl::eval_if<
                //8.5.3p5...
                is_rref<TestType>
              , mpl::if_<
                    mpl::or_<is_class<Type>, is_array<Type> >
                  , cv_type&&
                  , cv_type
                >
              , mpl::identity<cv_type&>
            >::type
        type;
#endif
    };

#ifndef BOOST_NO_RVALUE_REFERENCES
    //8.5.3p5...
    template <typename TestType,typename Type>
    struct forward_as<TestType,Type&&>
      : mpl::if_<
            mpl::or_<is_class<Type>, is_array<Type> >
          , Type&&
          , Type
        >
    {};
#endif

    //Optimization
    template <typename TestType,typename Type>
    struct forward_as<TestType,Type&>
    {
        typedef Type& type;
    };
}}}

#endif
