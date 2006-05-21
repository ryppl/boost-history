/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_CORE_IMPL_COMPOSITE_EVAL_IPP)
#define PHOENIX_CORE_IMPL_COMPOSITE_EVAL_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/enum.hpp>

    namespace impl
    {
        template <int N>
        struct composite_eval;

        template <>
        struct composite_eval<0>
        {
            template <typename Composite, typename Args, typename Env>
            struct apply
            {
                typedef typename Composite::eval_t::
                    template apply<Args, Env>::type
                type;
            };

            template <typename Composite, typename Args, typename Env>
            static typename apply<Composite, Args, Env>::type
            call(Composite const& composite, Args const& args, Env const& env)
            {
                typedef typename Composite::eval_t eval_t;
                return eval_t::eval(args, env);
            }
        };
    }

#define PHOENIX_GET_ACTOR_TYPE(z, n, data)                                      \
    typename fusion::tuple_element<n, Composite>::type

#define PHOENIX_GET_ACTOR(z, n, data)                                           \
    fusion::get<n>(composite)

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (1, PHOENIX_COMPOSITE_LIMIT,                                            \
    "boost/spirit/phoenix/core/impl/composite_eval.ipp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_GET_ACTOR
#undef PHOENIX_GET_ACTOR_TYPE
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
        struct composite_eval<N>
        {
            template <typename Composite, typename Args, typename Env>
            struct apply
            {
                typedef typename Composite::eval_t::
                    template apply<
                        Args
                      , Env
                      , BOOST_PP_ENUM(N, PHOENIX_GET_ACTOR_TYPE, _)
                    >::type
                type;
            };

            template <typename Composite, typename Args, typename Env>
            static typename apply<Composite, Args, Env>::type
            call(Composite const& composite, Args const& args, Env const& env)
            {
                typedef typename Composite::eval_t eval_t;
                return eval_t::eval(
                    args
                  , env
                  , BOOST_PP_ENUM(N, PHOENIX_GET_ACTOR, _));
            }
        };
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


