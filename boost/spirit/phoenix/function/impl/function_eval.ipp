/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_FUNCTION_IMPL_FUNCTION_EVAL_IPP)
#define PHOENIX_FUNCTION_IMPL_FUNCTION_EVAL_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>

    namespace impl
    {
        template <int N>
        struct function_eval;

        template <>
        struct function_eval<0>
        {
            template <typename Args, typename Env, typename F>
            struct apply
            {
                typedef typename F::template apply<Args, Env>::type function;
                typedef typename function::result_type type;
            };

            template <typename Args, typename Env, typename F>
            static typename apply<Args, Env, F>::type
            eval(Args const& args, Env const& env, F const& f)
            {
                return f.eval(args, env)();
            }
        };
    }

#define PHOENIX_GET_ARG(z, n, data)                                             \
    typedef typename BOOST_PP_CAT(A, n)::template apply<Args, Env>::type        \
        BOOST_PP_CAT(a, n);

#define PHOENIX_EVAL_ARG(z, n, data)                                            \
    BOOST_PP_CAT(_, n).eval(args, env)

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (1, BOOST_PP_DEC(PHOENIX_COMPOSITE_LIMIT),                              \
    "boost/spirit/phoenix/function/impl/function_eval.ipp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_GET_ARG
#undef PHOENIX_EVAL_ARG
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    namespace impl
    {
        template <>
        struct function_eval<N>
        {
            template <typename Args, typename Env, typename F
              , PHOENIX_TEMPLATE(N, A)>
            struct apply
            {
                typedef typename F::template apply<Args, Env>::type function;
                BOOST_PP_REPEAT(N, PHOENIX_GET_ARG, _);

                typedef typename function::
                    template apply<BOOST_PP_ENUM_PARAMS(N, a)>::type
                type;
            };

            template <typename Args, typename Env, typename F
              , PHOENIX_TEMPLATE(N, A)>
            static typename apply<Args, Env, F
              , BOOST_PP_ENUM_PARAMS(N, A)>::type
            eval(Args const& args, Env const& env, F const& f
              , PHOENIX_REF_ARGS(N, A))
            {
                return f.eval(args, env)(BOOST_PP_ENUM(N, PHOENIX_EVAL_ARG, _));
            }
        };
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


