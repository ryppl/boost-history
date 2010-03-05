/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    namespace result_of
    {
        VARIADIC_TEMPLATE(BOOST_FUSION_N)
        struct BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME(BOOST_PP_EMPTY()))
            EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
                BOOST_FUSION_MAX_SEQ_SIZE,BOOST_FUSION_N)
        {
            typedef
                BOOST_FUSION_SEQ_NAME(BOOST_FUSION_N)<
                    EXPAND_TEMPLATE_ARGUMENTS_BINARY(
                            BOOST_FUSION_N,
                            typename traits::deduce<,
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
        result_of::BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME(BOOST_PP_EMPTY()))<
            EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(BOOST_FUSION_N)
        >::type
    BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME(BOOST_PP_EMPTY()))(
        EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N))
    {
        return
#if BOOST_FUSION_N
            typename
#endif
            result_of::BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME(BOOST_PP_EMPTY()))<
                EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(BOOST_FUSION_N)
            >::type(EXPAND_PARAMETERS_A(BOOST_FUSION_N));
    }

#undef BOOST_FUSION_N

