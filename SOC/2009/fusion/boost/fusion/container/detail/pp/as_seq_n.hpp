/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    template <>
    struct BOOST_PP_CAT(BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl)<
        BOOST_FUSION_N
    >
    {
        template <typename It0>
        struct apply
        {
#if BOOST_FUSION_N
            typedef typename result_of::value_of<It0>::type T0;
#endif

#define BOOST_FUSION_IT_VALUE_ENUM(Z, N, _)\
            typedef typename\
                result_of::next<BOOST_PP_CAT(It, BOOST_PP_DEC(N))>::type\
            BOOST_PP_CAT(It, N);\
            typedef typename\
                result_of::value_of<BOOST_PP_CAT(It, N)>::type\
            BOOST_PP_CAT(T, N);

            BOOST_PP_REPEAT_FROM_TO(
                    1, BOOST_FUSION_N, BOOST_FUSION_IT_VALUE_ENUM, _)

#undef BOOST_FUSION_IT_VALUE_ENUM

            typedef
                BOOST_FUSION_SEQ_NAME<BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, T)>
            type;
        };
    };

#undef BOOST_FUSION_N

