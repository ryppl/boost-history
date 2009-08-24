/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PP_IS_ITERATING

#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/template.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        VARIADIC_TEMPLATE_WITH_DEFAULT(BOOST_FUSION_MAX_SEQ_SIZE)
        struct BOOST_FUSION_FUNCTION_NAME;
    }

#define BOOST_PP_FILENAME_1 <boost/fusion/container/generation/detail/pp/seq_tie.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_MAX_SEQ_SIZE)
#include BOOST_PP_ITERATE()

}}

#else

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    namespace result_of
    {
        VARIADIC_TEMPLATE(BOOST_FUSION_N)
        struct BOOST_FUSION_FUNCTION_NAME
            EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
                    BOOST_FUSION_MAX_SEQ_SIZE,BOOST_FUSION_N)
        {
            typedef
                BOOST_FUSION_SEQ_NAME<
                    EXPAND_TEMPLATE_ARGUMENTS_BINARY(
                            BOOST_FUSION_N,
                            typename detail::deduce_ref<,
                            >::type
                    )
                >
            type;
        };
    }

    VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
    inline
#if BOOST_FUSION_N
        typename
#endif
        result_of::BOOST_FUSION_FUNCTION_NAME<
            EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(BOOST_FUSION_N)
        >::type
    BOOST_FUSION_FUNCTION_NAME(
        EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_LREF(BOOST_FUSION_N))
    {
        return
#if BOOST_FUSION_N
            typename
#endif
            result_of::BOOST_FUSION_FUNCTION_NAME<
                EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(BOOST_FUSION_N)
            >::type(EXPAND_PARAMETERS_A(BOOST_FUSION_N));
    }

#undef N
#endif
