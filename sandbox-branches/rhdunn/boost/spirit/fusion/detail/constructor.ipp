/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(BOOST_FUSION_TUPLE_CONSTRUCTOR_IPP)
#define BOOST_FUSION_TUPLE_CONSTRUCTOR_IPP

#include <boost/preprocessor/iterate.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  The following Boost::PP code generate expansions for:
//
//      tuple(
//          typename tuple_detail::call_param<T0>::type _0,
//          typename tuple_detail::call_param<T1>::type _1,
//          ...
//          typename tuple_detail::call_param<TN>::type _N)
//          : data(_0, _1,... _N) {}
//
//  where N runs from 1 to BOOST_FUSION_MAX_ARITY
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_PP_ITERATION_PARAMS_1                   \
    (3, (3, BOOST_FUSION_MAX_ARITY,                   \
    "boost/spirit/fusion/detail/constructor.ipp"))
#include BOOST_PP_ITERATE()

#endif // defined(BOOST_FUSION_TUPLE_CONSTRUCTOR_IPP)

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    tuple(BOOST_FUSION_CALL_PARAMS(N))
        : data(BOOST_PP_ENUM_PARAMS(N, _)) {}

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
