/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONFIG_MAX_PARAMS_HPP
#define BOOST_ACT_CONFIG_MAX_PARAMS_HPP

#include <boost/fusion/sequence/container/vector/limits.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#ifndef BOOST_ACT_MAX_PARAMS
  #if !( FUSION_MAX_VECTOR_SIZE > 1 )
    #error In order to use Boost.Act, the max fusion vector size must be greater than 1. Increase the max vector size.
  #else
    #define BOOST_ACT_MAX_PARAMS BOOST_PP_DEC( FUSION_MAX_VECTOR_SIZE )
  #endif
#elif !( BOOST_ACT_MAX_PARAMS < FUSION_MAX_VECTOR_SIZE )
  #error Boost.Act Max Params is not less than the max fusion vector size. Increase the max vector size or decrease max params.
#elif BOOST_ACT_MAX_PARAMS == 0
  #error Boost.Act Max Params must be greater than 0. Increase the max params.
#endif

#endif
