/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_FUNCTION_IMPL_FUNCTION_CALL_IPP)
#define PHOENIX_FUNCTION_IMPL_FUNCTION_CALL_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, BOOST_PP_DEC(PHOENIX_COMPOSITE_LIMIT),                              \
    "boost/spirit/phoenix/function/impl/function_call.ipp"))
#include BOOST_PP_ITERATE()

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <PHOENIX_TEMPLATE(N, A)>
    actor<typename as_composite<impl::function_eval<1>, F
      , BOOST_PP_ENUM_PARAMS(N, A)>::type>
    operator()(PHOENIX_CREF_ARGS(N, A)) const
    {
        return compose<impl::function_eval<1> >(f, BOOST_PP_ENUM_PARAMS(N, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


