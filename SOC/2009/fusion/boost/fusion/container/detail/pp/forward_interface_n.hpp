/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

#ifndef BOOST_NO_RVALUE_REFERENCES
    VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
#endif
#if BOOST_FUSION_N == 1
    explicit
#endif
#ifdef BOOST_NO_RVALUE_REFERENCES
    BOOST_FUSION_SEQ_NAME(EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(BOOST_FUSION_N))
#else
    BOOST_FUSION_SEQ_NAME(EXPAND_TEMPLATE_ARGUMENTS_A(BOOST_FUSION_N))
#endif
      : BOOST_FUSION_INIT_BASE(EXPAND_PARAMETERS_A(BOOST_FUSION_N))
    {}

#undef BOOST_FUSION_N
