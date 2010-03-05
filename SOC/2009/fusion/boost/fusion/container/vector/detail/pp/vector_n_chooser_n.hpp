/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    VARIADIC_TEMPLATE(BOOST_FUSION_N)
    struct vector_n_chooser
        EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
            FUSION_MAX_VECTOR_SIZE,BOOST_FUSION_N)
    {
        typedef
            BOOST_PP_CAT(vector, BOOST_FUSION_N)<
                EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N)
            >
        type;
    };

#undef BOOST_FUSION_N

