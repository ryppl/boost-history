/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()


#ifdef BOOST_NO_RVALUE_REFERENCES
    explicit
    list(EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(BOOST_FUSION_N))
#else
    VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
#   if BOOST_FUSION_N!=1
    explicit
#   endif
    list(EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N))
#endif
#if BOOST_FUSION_N
#   if BOOST_FUSION_N <= 1
      : base_type(EXPAND_PARAMETERS_A(BOOST_FUSION_N))
#   else
#       ifdef BOOST_NO_RVALUE_REFERENCES
      : base_type(vector<
            BOOST_PP_ENUM_BINARY_PARAMS(
               BOOST_FUSION_N,
               typename call_traits<T,
               >::param_type BOOST_PP_INTERCEPT)
            >(EXPAND_PARAMETERS_A(BOOST_FUSION_N)))
#       else
      : base_type(fusion::vector_tie(EXPAND_PARAMETERS_A(BOOST_FUSION_N)))
#       endif
#   endif
#endif
    {}

#undef BOOST_FUSION_N