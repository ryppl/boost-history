/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_CORE_IMPL_ARG_LIST_CONSTRUCTOR_IPP)
#define PHOENIX_CORE_IMPL_ARG_LIST_CONSTRUCTOR_IPP

#include <boost/preprocessor/iterate.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, PHOENIX_ACTOR_LIMIT,                                                \
    "boost/spirit/phoenix/core/impl/arg_list_constructor.ipp"))
#include BOOST_PP_ITERATE()

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <PHOENIX_TEMPLATE(N, TB)>
    arg_list(PHOENIX_REF_ARGS(N, TB))
        : tie_t(PHOENIX_ARG_LIST(N)) {}

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


