/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PP_IS_ITERATING
#   ifndef BOOST_FUSION_SEQ_NAME
#       ifndef BOOST_FUSION_CONTAINER_DETAIL_PP_FORWARD_INTERFACE_HPP
#           define BOOST_FUSION_CONTAINER_DETAIL_PP_FORWARD_INTERFACE_HPP

#           include <boost/fusion/support/internal/template.hpp>

#           include <boost/config.hpp>
#           include <boost/preprocessor/iterate.hpp>
#           include <boost/preprocessor/repetition/enum_params.hpp>
#           include <boost/preprocessor/repetition/enum_binary_params.hpp>
#       endif
#   else
#       define BOOST_PP_FILENAME_1 <boost/fusion/container/detail/pp/forward_interface.hpp>
#       define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_MAX_SEQ_SIZE)
#       include BOOST_PP_ITERATE()
#   endif
#else
#   define BOOST_FUSION_N BOOST_PP_ITERATION()

#   ifndef BOOST_NO_RVALUE_REFERENCES
        VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
#   endif
#   if BOOST_FUSION_N == 1
        explicit
#   endif
#   ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_FUSION_SEQ_NAME(EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(BOOST_FUSION_N))
#   else
        BOOST_FUSION_SEQ_NAME(EXPAND_TEMPLATE_ARGUMENTS_A(BOOST_FUSION_N))
#   endif
          : BOOST_FUSION_INIT_BASE(EXPAND_PARAMETERS_A(BOOST_FUSION_N))
        {}

#   undef BOOST_FUSION_N
#endif
