/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_RESULT_OF_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_RESULT_OF_HPP

//cschmidt: fusion might call result_of on class types without an operator().
//Therefore we emulate the old behavior if (and only if) the boost
//implementation falls back to decltype by default.

#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace fusion { namespace detail
{
    //cschmidt: These metafunction needs to handle
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

    template<typename FRef>
    struct is_po_callable_impl
    {
        typedef typename detail::identity<FRef>::type f;
        typedef typename is_pointer<f>::type is_pointer;

        typedef
            mpl::or_<
                is_function<typename remove_pointer<f>::type>
              , is_member_function_pointer<f>
            >
        type;
    };

    template<typename FRef>
    struct is_po_callable
      : is_po_callable_impl<FRef>::type
    {
        typedef typename is_po_callable_impl<FRef>::is_pointer is_pointer;
    };

//cschmidt: a pp implementation won't be worth the effort
#ifdef BOOST_NO_VARIADIC_TEMPLATES
    template<typename FRef>
    struct is_preevaluable
      : mpl::false_
    {};

    template<typename FRef>
    struct preevaluate;
#else
    template<typename FRef>
    struct is_preevaluable
      : is_po_callable<FRef>
    {};

    template<typename FRef>
    struct preevaluate_impl;

    template<typename Result,typename... Args>
    struct preevaluate_impl<Result (Args...)>
    {
        typedef Result type;
    };

    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...)>
    {
        typedef Class& class_type;

        typedef Result type;
    };

    //cschmidt: Once we get a macro to find out whether the compiler
    //supports rvalue references for this, use
    //BOOST_FUSION_ALL_CV_REF_COMBINATIONS
    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...) const>
    {
        typedef Class const& class_type;

        typedef Result type;
    };

    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...) const volatile>
    {
        typedef Class const volatile& class_type;

        typedef Result type;
    };

    template<typename Result,typename Class,typename... Args>
    struct preevaluate_impl<Result(Class::*)(Args...) volatile>
    {
        typedef Class volatile& class_type;

        typedef Result type;
    };

    template<typename FRef>
    struct preevaluate
      : preevaluate_impl<
            typename remove_pointer<
                typename detail::identity<FRef>::type
            >::type
        >
    {};
#endif

    //cschmidt: The non-decltype result_of does not like ref-qualified
    //'class type' functions
    template<typename FRef>
    struct get_func_base
    {
        typedef typename
            remove_pointer<
                typename detail::identity<FRef>::type
            >::type
        f;

        typedef typename
            mpl::if_<
                mpl::or_<
                   typename is_function<f>::type
                 , typename is_member_function_pointer<f>::type
                >
              , FRef
              , f
            >::type
        type;
    };
}}}

#endif
