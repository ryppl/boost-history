/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_RESULT_OF_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_RESULT_OF_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#if defined(BOOST_NO_DECLTYPE) || !defined(BOOST_NO_VARIADIC_TEMPLATES)
#   include <boost/mpl/bool.hpp>
#endif
#ifdef BOOST_NO_DECLTYPE
#   include <boost/mpl/has_xxx.hpp>
#   include <boost/mpl/eval_if.hpp>
#   include <boost/mpl/identity.hpp>
#endif
#ifndef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/mpl/identity.hpp>
#endif
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace fusion { namespace detail
{
    //cschmidt: This metafunction needs to handle
    //T()
    //T(&)()
    //T(cv&)()
    //T(*)()
    //T(*cv)()
    //T(*cv&)()
    //T(C::*)()
    //T(C::*cv)()
    //T(C::*&)()
    //T(C::*cv&)()
    //T(C::*)()cv
    //T(C::*cv)()cv
    //T(C::*&)()cv
    //T(C::*cv&)()cv
    //T(C::*)()cv&
    //T(C::*cv)()cv&
    //T(C::*&)()cv&
    //T(C::*cv&)()cv&
    //& -> &&

    template<typename F>
    struct is_po_callable
    {
        typedef typename identity<F>::type identity_f;
        typedef typename is_pointer<identity_f>::type is_pointer;
        typedef typename remove_pointer<identity_f>::type f;

        typedef
            mpl::or_<
                is_function<f>
              , is_member_function_pointer<f>
            >
        type;
    };

#ifdef BOOST_NO_DECLTYPE
    BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type);

    template<typename F>
    struct get_result_type
    {
        typedef typename F::result_type type;
    };

    template<typename F>
    struct preevaluate_impl
    {
        typedef typename has_result_type<F>::type is_preevaluable;
        typedef typename
            mpl::eval_if<
                is_preevaluable
              , get_result_type<F>
              , mpl::identity<void>
            >::type
        type;
    };
#else
    template<typename F>
    struct preevaluate_impl
    {
        typedef mpl::false_ is_preevaluable;
        typedef void type;
    };
#endif

    //cschmidt: a pp implementation won't be worth the effort
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    template<typename Result,typename... Args>
    struct preevaluate_impl<Result (Args...)>
    {
        typedef mpl::true_ is_preevaluable;
        typedef Result type;
    };

    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...)>
    {
        typedef Class& class_type;

        typedef mpl::true_ is_preevaluable;
        typedef Result type;
    };

    //cschmidt: Once we get a macro to find out whether the compiler
    //supports rvalue references for this, use
    //BOOST_FUSION_ALL_CV_REF_COMBINATIONS
    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...) const>
    {
        typedef Class const& class_type;
        typedef mpl::true_ is_preevaluable;
        typedef Result type;
    };

    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...) const volatile>
    {
        typedef Class const volatile& class_type;
        typedef mpl::true_ is_preevaluable;
        typedef Result type;
    };

    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...) volatile>
    {
        typedef Class volatile& class_type;
        typedef mpl::true_ is_preevaluable;
        typedef Result type;
    };
#endif

    template<typename F>
    struct preevaluate
    {
#if defined(BOOST_NO_VARIADIC_TEMPLATES) && !defined(BOOST_NO_DECLTYPE)
        typedef mpl::false_ is_preevaluable;
#else
        typedef
            preevaluate_impl<
                typename remove_pointer<typename identity<F>::type>::type
            >
        gen;
        typedef typename gen::type type;
        typedef typename gen::is_preevaluable is_preevaluable;
#endif
    };

    //cschmidt: boost::result_of does not like ref-qualified
    //'class type' functors
    template<typename F>
    struct get_func_base
    {
        typedef is_po_callable<F> is_po_callable_;

        typedef typename
            mpl::if_<
                typename is_po_callable_::type
              , typename is_po_callable_::f&
              , typename is_po_callable_::identity_f
            >::type
        type;
    };
}}}

#endif
