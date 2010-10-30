/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    namespace result_of
    {
        VARIADIC_TEMPLATE(BOOST_FUSION_N)
        struct BOOST_FUSION_FUNCTION_NAME
            EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
                    BOOST_FUSION_MAX_SEQ_SIZE,BOOST_FUSION_N)
        {
            typedef
                BOOST_FUSION_SEQ_NAME(BOOST_FUSION_N)<
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
            >::type(
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
                EXPAND_PARAMETERS_A(BOOST_FUSION_N)
#else

#   define BOOST_FUSION_SEQ_TIE_FORWARD_ARGS(Z,N,__)\
                static_cast<\
                    typename detail::deduce_ref<BOOST_PP_CAT(A,N)&&>::type\
                >(BOOST_PP_CAT(_,N))

                BOOST_PP_ENUM(
                    BOOST_FUSION_N,BOOST_FUSION_SEQ_TIE_FORWARD_ARGS,_)

#   undef BOOST_FUSION_MAP_TIE_FORWARD_ARGS

#endif
            );
    }

#undef BOOST_FUSION_N

