/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_OBJECT_IMPL_CONSTRUCT_EVAL_IPP)
#define PHOENIX_OBJECT_IMPL_CONSTRUCT_EVAL_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>

#define PHOENIX_EVAL_ARG(z, n, data)                                            \
    BOOST_PP_CAT(_, n).eval(args, env)

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, PHOENIX_COMPOSITE_LIMIT,                                            \
    "boost/spirit/phoenix/object/impl/construct_eval.ipp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_EVAL_ARG
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <typename Args, typename Env, PHOENIX_TEMPLATE(N, A)>
    static T
    eval(Args const& args, Env const& env, PHOENIX_REF_ARGS(N, A))
    {
        return T(BOOST_PP_ENUM(N, PHOENIX_EVAL_ARG, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


