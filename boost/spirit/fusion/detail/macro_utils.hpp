/*=============================================================================
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2001-2003 Jaakko Järvi

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_MACRO_UTILS_HPP)
#define BOOST_FUSION_MACRO_UTILS_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  BOOST_FUSION_TEMPLATE_PARAMS(n, T) expands to
//
//      typename T0, typename T1, ... typename TN
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_FUSION_TEMPLATE_PARAM(z, n, data)                                 \
    typename BOOST_PP_CAT(data, n)
#define BOOST_FUSION_TEMPLATE_PARAMS(n, prefix)                                 \
    BOOST_PP_ENUM(n, BOOST_FUSION_TEMPLATE_PARAM, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  BOOST_FUSION_TEMPLATE_PARAMS_WITH_DEFAULT(n, void_t) expands to
//
//      typename T0 = void_t, typename T1 = void_t, ... typename TN = void_t
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_FUSION_TEMPLATE_PARAM_WITH_DEFAULT(z, n, data)                    \
    typename BOOST_PP_CAT(T, n) = data
#define BOOST_FUSION_TEMPLATE_PARAMS_WITH_DEFAULT(n, default_)                  \
    BOOST_PP_ENUM(n, BOOST_FUSION_TEMPLATE_PARAM_WITH_DEFAULT, default_)

///////////////////////////////////////////////////////////////////////////////
//
//  BOOST_FUSION_CALL_PARAMS(n) expands to
//
//        typename tuple_detail::call_param<T0>::type
//      , typename tuple_detail::call_param<T1>::type
//      ...
//      , typename tuple_detail::call_param<TN>::type
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_FUSION_CALL_PARAM(z, n, data)                                     \
    typename tuple_detail::call_param<BOOST_PP_CAT(T, n)>::type                 \
        BOOST_PP_CAT(_, n)
#define BOOST_FUSION_CALL_PARAMS(n)                                             \
    BOOST_PP_ENUM(n, BOOST_FUSION_CALL_PARAM, _)

///////////////////////////////////////////////////////////////////////////////
//
//  BOOST_FUSION_TEMPLATE_ARGS(T) expands to
//
//      T0, T1, ... TN
//
//  where N == BOOST_FUSION_MAX_ARITY
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_FUSION_TEMPLATE_ARGS(prefix)                                      \
    BOOST_PP_ENUM_PARAMS(BOOST_FUSION_MAX_ARITY, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  BOOST_FUSION_TEMPLATE_PARAMS_ALL(T) expands to
//
//      typename T0, typename T1, ... typename TN
//
//  where N == BOOST_FUSION_MAX_ARITY
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_FUSION_TEMPLATE_PARAMS_ALL(prefix)                                \
    BOOST_FUSION_TEMPLATE_PARAMS(BOOST_FUSION_MAX_ARITY, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  BOOST_FUSION_TUPLE(T) expands to
//
//      tuple<T0, T1, ... TN, ID>
//
//  where N == BOOST_FUSION_MAX_ARITY
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_FUSION_TUPLE(prefix)                                              \
    tuple<BOOST_FUSION_TEMPLATE_ARGS(prefix), ID>

#endif // defined(BOOST_FUSION_MACRO_UTILS_HPP)
