/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_STATEMENT_IMPL_SWITCH_EVAL_IPP)
#define PHOENIX_STATEMENT_IMPL_SWITCH_EVAL_IPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>

#define PHOENIX_CASE_ITEM(z, n, prefix)                                         \
    case BOOST_PP_CAT(Case, n)::value:                                          \
        BOOST_PP_CAT(_, n).eval(args, env);                                     \
        break;

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, BOOST_PP_DEC(BOOST_PP_DEC(PHOENIX_COMPOSITE_LIMIT)),                \
    "boost/spirit/phoenix/statement/impl/switch_eval.ipp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_CASE_ITEM
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <>
    struct switch_eval<N>
    {
        template <
            typename Args, typename Env
          , typename Cond, typename Default
          , PHOENIX_TEMPLATE(N, Case)
        >
        struct apply
        {
            typedef void type;
        };

        template <
            typename Args, typename Env
          , typename Cond, typename Default
          , PHOENIX_TEMPLATE(N, Case)
        >
        static void
        eval(
            Args const& args, Env const& env
          , Cond& cond, Default& default_
          , PHOENIX_REF_ARGS(N, Case)
        )
        {
            switch (cond.eval(args, env))
            {
                BOOST_PP_REPEAT(N, PHOENIX_CASE_ITEM, _)
                default:
                    default_.eval(args, env);
            }
        }
    };

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


