/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(PHOENIX_CORE_IMPL_MACRO_UTILS_HPP)
#define PHOENIX_CORE_IMPL_MACRO_UTILS_HPP

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_TEMPLATE(n, T) expands to
//
//      typename T0, typename T1, ... template TN
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_TEMPLATE_PARAM(z, n, prefix)                                    \
    typename BOOST_PP_CAT(prefix, n)
#define PHOENIX_TEMPLATE(n, prefix)                                             \
    BOOST_PP_ENUM(n, PHOENIX_TEMPLATE_PARAM, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_TEMPLATE_WITH_DEFAULT(n, T) expands to
//
//      typename T0 = void_t, typename T1 = void_t, ... template TN = void_t
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_TEMPLATE_PARAM_WITH_DEFAULT(z, n, prefix)                       \
    typename BOOST_PP_CAT(prefix, n) = void_t
#define PHOENIX_TEMPLATE_WITH_DEFAULT(n, prefix)                                \
    BOOST_PP_ENUM(n, PHOENIX_TEMPLATE_PARAM_WITH_DEFAULT, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_CALL_ARGS(n, T) expands to
//
//      typename call_traits<T0>::param_type _0,
//      typename call_traits<T1>::param_type _1,
//      ...
//      typename call_traits<TN>::param_type _N
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_CALL_ARG(z, n, prefix)                                          \
    typename call_traits<BOOST_PP_CAT(prefix, n)>                               \
        ::param_type BOOST_PP_CAT(_, n)
#define PHOENIX_CALL_ARGS(n, prefix)                                            \
    BOOST_PP_ENUM(n, PHOENIX_CALL_ARG, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_CALL_ARGS_WITH_DEFAULT(n, T) expands to
//
//      typename call_traits<T0>::param_type _0 = T0(),
//      typename call_traits<T1>::param_type _1 = T1(),
//      ...
//      typename call_traits<TN>::param_type _N = TN()
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_CALL_ARG_WITH_DEFAULT(z, n, prefix)                             \
    typename call_traits<BOOST_PP_CAT(prefix, n)>                               \
        ::param_type BOOST_PP_CAT(_, n) = BOOST_PP_CAT(prefix, n)()
#define PHOENIX_CALL_ARGS_WITH_DEFAULT(n, prefix)                               \
    BOOST_PP_ENUM(n, PHOENIX_CALL_ARG_WITH_DEFAULT, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_CREF_ARGS(n, T) expands to
//
//      T0 const& _0, T1 const& _1, ...TN const& _N
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_CREF_ARG(z, n, prefix)                                          \
    BOOST_PP_CAT(prefix, n) const& BOOST_PP_CAT(_, n)
#define PHOENIX_CREF_ARGS(n, prefix)                                            \
    BOOST_PP_ENUM(n, PHOENIX_CREF_ARG, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_CREF_ARGS_WITH_DEFAULT(n, T) expands to
//
//      T0 const& _0 = T0(), T1 const& _1 = T1(), ... TN const& _N = TN()
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_CREF_ARG_WITH_DEFAULT(z, n, prefix)                             \
    BOOST_PP_CAT(prefix, n) const& BOOST_PP_CAT(_, n)                           \
        = BOOST_PP_CAT(prefix, n)()
#define PHOENIX_CREF_ARGS_WITH_DEFAULT(n, prefix)                               \
    BOOST_PP_ENUM(n, PHOENIX_CREF_ARG_WITH_DEFAULT, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_REF_ARGS(n, T) expands to
//
//      T0& _0, T1& _1, ...TN& _N
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_REF_ARG(z, n, prefix)                                           \
    BOOST_PP_CAT(prefix, n)& BOOST_PP_CAT(_, n)
#define PHOENIX_REF_ARGS(n, prefix)                                             \
    BOOST_PP_ENUM(n, PHOENIX_REF_ARG, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_REF_LIST(n, T) expands to
//
//      T0&, T1&, ...TN&
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_REF_ELEM(z, n, prefix)                                          \
    BOOST_PP_CAT(prefix, n)&
#define PHOENIX_REF_LIST(n, prefix)                                             \
    BOOST_PP_ENUM(n, PHOENIX_REF_ELEM, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_TIE_LIST(n, T) expands to
//
//      T0&, T1&, ...TN&
//
//  Note: Reference(&) is added to the type *ONLY* if T is not a void_t
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_TIE_ELEM(z, n, prefix)                                          \
    typename mpl::if_<                                                          \
        is_same<void_t, BOOST_PP_CAT(prefix, n)>                                \
      , void_t                                                                  \
      , BOOST_PP_CAT(prefix, n)&                                                \
    >::type
#define PHOENIX_TIE_LIST(n, prefix)                                             \
    BOOST_PP_ENUM(n, PHOENIX_TIE_ELEM, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_TYPE_LIST(n, T) expands to
//
//      T0, T1, ... TN
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_T_ITEM(z, n, prefix)                                            \
    BOOST_PP_CAT(prefix, n)
#define PHOENIX_TYPE_LIST(n, prefix)                                            \
    BOOST_PP_ENUM(n, PHOENIX_T_ITEM, prefix)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_ARG_LIST(n) expands to
//
//      _0, _1, ... _N
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_ARG_ITEM(z, n, data)                                            \
    BOOST_PP_CAT(_, n)
#define PHOENIX_ARG_LIST(n)                                                     \
    BOOST_PP_ENUM(n, PHOENIX_ARG_ITEM, _)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_INIT_MEMBERS(n) expands to
//
//      m0(_0), m1(_1), ..., mN(_N)
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_INIT_MEMBER(z, n, data)                                         \
    BOOST_PP_CAT(m, n)(BOOST_PP_CAT(_, n))
#define PHOENIX_INIT_MEMBERS(n)                                                 \
    BOOST_PP_ENUM(n, PHOENIX_INIT_MEMBER, _)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_DEFAULT_INIT_MEMBERS(n) expands to
//
//      m0(), m1(), ..., mN()
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_DEFAULT_INIT_MEMBER(z, n, data)                                \
    BOOST_PP_CAT(m, n)()
#define PHOENIX_DEFAULT_INIT_MEMBERS(n)                                        \
    BOOST_PP_ENUM(n, PHOENIX_DEFAULT_INIT_MEMBER, _)

///////////////////////////////////////////////////////////////////////////////
//
//  PHOENIX_DECLARE_MEMBERS(n) expands to
//
//      T0 m0;
//      T1 m1;
//      ...
//      TN mN;
//
///////////////////////////////////////////////////////////////////////////////
#define PHOENIX_DECLARE_MEMBER(z, n, data)                                      \
    BOOST_PP_CAT(T, n) BOOST_PP_CAT(m, n) ;
#define PHOENIX_DECLARE_MEMBERS(n)                                              \
    BOOST_PP_REPEAT(n, PHOENIX_DECLARE_MEMBER, _)

#endif
