/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_CORE_IMPL_COMPOSE_IPP)
#define PHOENIX_CORE_IMPL_COMPOSE_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#define PHOENIX_AS_ACTOR_CONVERT(z, n, data)                                    \
    as_actor<BOOST_PP_CAT(T, n)>::convert(BOOST_PP_CAT(_, n))

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, PHOENIX_COMPOSITE_LIMIT,                                            \
    "boost/spirit/phoenix/core/impl/compose.ipp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_AS_ACTOR_CONVERT
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <typename Eval, PHOENIX_TEMPLATE(N, T)>
    inline actor<
        typename as_composite<Eval, PHOENIX_TYPE_LIST(N, T)>::type>
    compose(PHOENIX_CREF_ARGS(N, T))
    {
        return actor<
            typename as_composite<Eval, PHOENIX_TYPE_LIST(N, T)>::type>(
                BOOST_PP_ENUM(N, PHOENIX_AS_ACTOR_CONVERT, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


