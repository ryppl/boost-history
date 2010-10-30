/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    explicit
    BOOST_FUSION_SEQ_NAME(EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(BOOST_FUSION_N))
#else
    VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
#   if BOOST_FUSION_N!=1
    explicit
#   endif
    BOOST_FUSION_SEQ_NAME(\
        EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N))
#endif
      : BOOST_FUSION_INIT_BASE(EXPAND_PARAMETERS_A(BOOST_FUSION_N))
    {}

#undef BOOST_FUSION_N
