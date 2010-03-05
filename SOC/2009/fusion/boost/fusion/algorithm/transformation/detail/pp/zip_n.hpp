/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    namespace result_of
    {
        VARIADIC_TEMPLATE(BOOST_FUSION_N)
        struct zip
            EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
                    FUSION_MAX_ZIP_SEQUENCES,BOOST_FUSION_N)
        {
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERS
#define BOOST_FUSION_IS_FORWARD_ASSERT(Z,N,_)\
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<BOOST_PP_CAT(T,N)>));

            BOOST_PP_REPEAT(BOOST_FUSION_N,BOOST_FUSION_IS_FORWARD_ASSERT,_)

#undef BOOST_FUSION_IS_FORWARD_ASSERT
#endif

            typedef
                zip_view<
                    typename result_of::vector_tie<
                        EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N)
                    >::type
                >
            type;
        };
    }

    VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
    inline
#if BOOST_FUSION_N
        typename
#endif
        result_of::zip<
            EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(BOOST_FUSION_N)
        >::type
    zip(EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N))
    {
        return
#if BOOST_FUSION_N
            typename
#endif
            result_of::zip<
                EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(BOOST_FUSION_N)
            >::type(fusion::vector_tie(EXPAND_PARAMETERS_A(BOOST_FUSION_N)));
    }

#undef BOOST_FUSION_N
