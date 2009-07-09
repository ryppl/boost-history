// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_TEMPLATE_HPP
#define BOOST_FUSION_SUPPORT_TEMPLATE_HPP

#include <boost/config.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#endif

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define VARIADIC_TEMPLATE_WITH_DEFAULT(MAX) struct void_;\
    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(MAX, typename T, void_)>
#   define VARIADIC_TEMPLATE(MAX) struct void;\
    template<BOOST_PP_ENUM_PARAMS(MAX, typename T)>
#   define EXPAND_ARGUMENTS(MAX) BOOST_PP_ENUM_PARAMS(MAX, T)
#else
#   define VARIADIC_TEMPLATE_WITH_DEFAULT(MAX) template<typename... Arguments>
#   define VARIADIC_TEMPLATE(MAX) template<typename... Arguments>
#   define EXPAND_ARGUMENTS(MAX) Arguments...
#endif

#endif
