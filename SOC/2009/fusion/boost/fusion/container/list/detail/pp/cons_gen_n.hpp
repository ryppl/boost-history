/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    VARIADIC_TEMPLATE(BOOST_FUSION_N)
    struct cons_gen
        EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
            FUSION_MAX_LIST_SIZE,BOOST_FUSION_N)
    {
    public:
#if BOOST_FUSION_N
        typedef
            cons<
                T0
              , typename cons_gen<
                    BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_FUSION_N, T)
                >::type
            >
        type;
#else
        typedef nil type;
#endif
    };

#undef BOOST_FUSION_N

