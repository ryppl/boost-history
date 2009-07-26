// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_RESULT_OF_HPP
#define BOOST_FUSION_SUPPORT_RESULT_OF_HPP

#include <boost/utility/result_of.hpp>

//cschmidt: fusion might call result_of on class types without an operator().
//Therefore we emulate the old behavior if (and only if) the boost
//implementation falls back to decltype by default.

#include <boost/fusion/support/ref.hpp>

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>

#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   include <boost/mpl/if.hpp>
#else
#   include <boost/fusion/support/ref.hpp>

#   include <boost/mpl/eval_if.hpp>
#   include <boost/mpl/has_xxx.hpp>

#   include <boost/type_traits/is_function.hpp>
#endif

namespace boost { namespace fusion { namespace support
{
//cschmidt: a pp implementation won't be worth the effort
#ifdef BOOST_NO_VARIADIC_TEMPLATES
    template<typename FRef>
    struct is_preevaluable
      : mpl::false_
    {};

    template<typename FRef>
    struct preevaluate;
#else
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
    struct is_preevaluable
    {
         typedef typename
             remove_pointer<
                 typename detail::identity<FRef>::type
             >::type
         f;

         typedef typename
             mpl::or_<
                typename is_function<f>::type
              , typename is_member_function_pointer<f>::type
             >::type
          type;
    };

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

    //TODO cschmidt: Once we got a macro to find out whether the compiler
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

#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
    using boost::result_of;

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
#else
    template<typename F>
    struct get_func_base
    {
        typedef F f;
    };

    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

        template<typename IdentityF>
        struct get_result_type
        {
            typedef typename IdentityF::result_type type;
        };

        template<typename IdentityF, typename Sig>
        struct get_result
          : IdentityF::template result<Sig>
        {};

        //TODO cschmidt: non-native variadic templates version
        template<typename F,typename IdentityF, typename... Args>
        struct result_of_class_type
        {
            typedef typename
                mpl::eval_if<
                    detail::has_result_type<IdentityF>
                  , detail::get_result_type<IdentityF>
                    //TODO cschmidt: fallback to boost::result_of (decltype) if
                    //'F::template result' does not exist.
                    //Is this even possible?
                  , detail::get_result<IdentityF,F(Args...)>
                >::type
            type;
        };
    }

    template<typename Sig>
    struct result_of
      : boost::result_of<Sig>
    {
    };

    template<typename F, typename... Args>
    struct result_of<F(Args...)>
    {
        //TODO cschmidt: workaround until is_function supports rvalues
        typedef typename fusion::detail::identity<F>::type f;

        typedef typename
            mpl::eval_if<
                is_function<f>
              , boost::result_of<f(Args...)>
              , detail::result_of_class_type<F,f,Args...>
            >::type
        type;
    };
#endif

}}}

#endif
