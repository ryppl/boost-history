/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PP_IS_ITERATING
#   define BOOST_PP_FILENAME_1 <boost/fusion/container/list/detail/pp/list_forward_ctor.hpp>
#   define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_LIST_SIZE)
#   include BOOST_PP_ITERATE()
#else
#   define BOOST_FUSION_N BOOST_PP_ITERATION()

#   ifndef BOOST_NO_RVALUE_REFERENCES
    VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
#   endif
#   if BOOST_FUSION_N == 1
    explicit
#   endif
#   ifdef BOOST_NO_RVALUE_REFERENCES
    list(EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(BOOST_FUSION_N))
#   else
    list(EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N))
#   endif
#   if BOOST_FUSION_N
#       if BOOST_FUSION_N <= 1
      : base_type(EXPAND_PARAMETERS_A(BOOST_FUSION_N))
#       else
#           ifdef BOOST_NO_RVALUE_REFERENCES
      : base_type(vector<
                      BOOST_PP_ENUM_BINARY_PARAMS(BOOST_FUSION_N,
                              typename call_traits<T,
                              >::param_type BOOST_PP_INTERCEPT)
                  >(EXPAND_PARAMETERS_A(BOOST_FUSION_N)))
#           else
      : base_type(vector<
                      BOOST_PP_ENUM_BINARY_PARAMS(N, A,&& BOOST_PP_INTERCEPT)
                  >(EXPAND_PARAMETERS_A(BOOST_FUSION_N)))
#           endif
#       endif
#   endif
    {}

#   undef BOOST_FUSION_N
#endif
