/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_TEMPLATE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_TEMPLATE_HPP

#include <boost/config.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/support/internal/ref.hpp>

#   include <boost/preprocessor/expand.hpp>
#   include <boost/preprocessor/empty.hpp>
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/control/if.hpp>
#   include <boost/preprocessor/facilities/intercept.hpp>
#   include <boost/preprocessor/comparison/equal.hpp>
#   include <boost/preprocessor/arithmetic/sub.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#   include <boost/call_traits.hpp>
#endif

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define DUMMY1(_)
#   define DUMMY2(_,__)

    //cschmidt: for clarity’s sake no linebreaks and no BOOST_FUSION_-prefix . TODO!!!
#   define VARIADIC_TEMPLATE(N)                 template<BOOST_PP_ENUM_PARAMS(N, typename T)>
#   define VARIADIC_TEMPLATE_WITH_DEFAULT(N)    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(N, typename T, void_)>
#   define EXPAND_TEMPLATE_ARGUMENTS(N)         BOOST_PP_ENUM_PARAMS(N, T)
#   ifdef BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#       define EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION_IMPL(MAX,N)<EXPAND_TEMPLATE_ARGUMENTS(N), BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(MAX,N), void_)>
#   else
#       define EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION_IMPL(MAX,N)<EXPAND_TEMPLATE_ARGUMENTS(N)>
#   endif
#   define EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(MAX,N)BOOST_PP_IF(BOOST_PP_EQUAL(MAX,N),DUMMY2,EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION_IMPL)(MAX,N)
#   define EXPAND_TEMPLATE_ARGUMENTS_BINARY(N,PRE,POST)BOOST_PP_ENUM_BINARY_PARAMS(N, PRE T, POST BOOST_PP_INTERCEPT)
#   define EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(N)BOOST_PP_ENUM_BINARY_PARAMS(N, typename call_traits<T, >::param_type _)
#   define EXPAND_PARAMETERS(N)                 BOOST_PP_ENUM_PARAMS(N, _)

#   define VARIADIC_TEMPLATE_A_IMPL(N)          template<BOOST_PP_ENUM_PARAMS(N, typename A)>
#   define VARIADIC_TEMPLATE_A(N)               BOOST_PP_IF(N,VARIADIC_TEMPLATE_A_IMPL,DUMMY1)(N)
#   define EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(N)BOOST_PP_ENUM_BINARY_PARAMS(N, A, BOOST_FUSION_R_ELSE_LREF(BOOST_PP_EMPTY()) BOOST_PP_INTERCEPT)
#   define EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(N)BOOST_PP_ENUM_BINARY_PARAMS(N, A,BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()) BOOST_PP_INTERCEPT)
#   define EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_LREF(N)BOOST_PP_ENUM_BINARY_PARAMS(N, A, BOOST_FUSION_R_ELSE_LREF(BOOST_PP_EMPTY()) _)
#   define EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(N)BOOST_PP_ENUM_BINARY_PARAMS(N, A,BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()) _)
#   define EXPAND_PARAMETERS_A_IMPL(Z,N,__)     BOOST_FUSION_FORWARD(BOOST_PP_CAT(A,N),BOOST_PP_CAT(_,N))
#   define EXPAND_PARAMETERS_A(N)               BOOST_PP_ENUM(N,EXPAND_PARAMETERS_A_IMPL,_)

namespace boost { namespace fusion
{
    struct void_;
}}
#else
#   define VARIADIC_TEMPLATE_WITH_DEFAULT(MAX) template<typename... Arguments>
#   define VARIADIC_TEMPLATE(MAX) template<typename... Arguments>
#   define EXPAND_TEMPLATE_ARGUMENTS(MAX) Arguments...
#endif

#endif
