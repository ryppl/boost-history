/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_MACRO_DEF_HPP_
#define _BOOST_OVERLOAD_DETAIL_MACRO_DEF_HPP_


#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>


#define OVL_DEFAULT_TEMPL_ARG(z, n, unused) \
detail::no_signature
// end macro OVL_DEFAULT_TEMPL_ARG

#define OVL_TEMPL_PARAMS(z, n, T) \
typename BOOST_PP_CAT(T, n) = OVL_DEFAULT_TEMPL_ARG(z, n, unused)
// end macro OVL_TEMPL_PARAMS

#define OVL_SET_IMPL(z,  n,  f) \
set_impl( BOOST_PP_CAT(f, n) );                                                \

// end macro OVL_ASSIGN_TO

#define OVL_SET(z, n, unused) \
template< BOOST_PP_ENUM_PARAMS(n, typename F) >                                \
this_type& set( BOOST_PP_ENUM_BINARY_PARAMS(n, F, const& _f) )                 \
{                                                                              \
    BOOST_PP_REPEAT(n, OVL_SET_IMPL, _f)                                       \
    return *this;                                                              \
}                                                                              \

// end macro OVL_CTOR

#define OVL_CTOR(z, n, unused) \
template< BOOST_PP_ENUM_PARAMS(n, typename F) >                                \
overload( BOOST_PP_ENUM_BINARY_PARAMS(n, F, const& _f) )                       \
{                                                                              \
    set( BOOST_PP_ENUM_PARAMS(n, _f) );                                        \
}                                                                              \

// end macro OVL_CTOR


///////////////////////////////////////////////////////////////////////////////
// Workarounds for MSVC < 8.0 tested on MSVC 7.1 

#if BOOST_WORKAROUND(BOOST_MSVC, < 1400)  

#define OVL_REAL_OR_DUMMY_SIG_M_N(z, m, n) \
typedef                                                                        \
    typename detail::real_or_dummy_signature<n, BOOST_PP_CAT(Sig, n), m>::type \
    BOOST_PP_CAT(sig_, BOOST_PP_CAT(n, BOOST_PP_CAT(_, BOOST_PP_CAT(m, _t)))); \

//end macro OVL_REAL_OR_DUMMY_SIG_M_N

#define OVL_REAL_OR_DUMMY_SIG_M(z, n, unused) \
BOOST_PP_REPEAT(BOOST_OVERLOAD_MAX_NUM_ARGS, OVL_REAL_OR_DUMMY_SIG_M_N, n)
// end macro OVL_REAL_OR_DUMMY_SIG_M

#define OVL_FC_OP_PARAM(z, m, tuple) \
typename detail::func_trait<                                                   \
    BOOST_PP_CAT(sig_,                                                         \
        BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, tuple),                         \
            BOOST_PP_CAT(_,                                                    \
                BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1, tuple), _t)             \
            )                                                                  \
         )                                                                     \
    )                                                                          \
>::BOOST_PP_CAT(arg, BOOST_PP_CAT(m, _type)) BOOST_PP_CAT(a, m)                \

// end macro OVL_FC_OP_PARAM

#define OVL_FUNC_CALL_OPERATOR_M_N(z, m, n) \
typename detail::func_trait<BOOST_PP_CAT(Sig,n)>::result_type                  \
operator() ( BOOST_PP_ENUM_SHIFTED(BOOST_PP_INC(m), OVL_FC_OP_PARAM, (n, m)) ) \
{                                                                              \
    typedef                                                                    \
        BOOST_PP_CAT(sig_,BOOST_PP_CAT(n,BOOST_PP_CAT(_,BOOST_PP_CAT(m, _t)))) \
        signature_type;                                                        \
    return boost::function<signature_type>::                                   \
           operator()( BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(m), a) );     \
}                                                                              \

// end macro OVL_FUNC_CALL_OPERATOR_M_N

#define OVL_FUNC_CALL_OPERATORS(z, n, unused) \
BOOST_PP_REPEAT_FROM_TO(1, BOOST_OVERLOAD_MAX_NUM_ARGS, OVL_FUNC_CALL_OPERATOR_M_N, n)
// end macro OVL_FUNC_CALL_OPERATORS

#endif
// end workarounds for MSVC < 8.0 tested on MSVC 7.1
///////////////////////////////////////////////////////////////////////////////


#endif // _BOOST_OVERLOAD_DETAIL_MACRO_DEF_HPP_

