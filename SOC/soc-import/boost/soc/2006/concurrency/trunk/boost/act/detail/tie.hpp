//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_TIE_HPP
#define BOOST_ACT_DETAIL_TIE_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

#define BOOST_ACT_DETAIL_TIE_FUNCTION_TEMPLATE_HEADER( z, num_params )         \
template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) > 

#define BOOST_ACT_DETAIL_TIE_FUNCTION_NO_TEMPLATE_HEADER( z, num_params ) 

#define BOOST_ACT_DETAIL_TIE_FUNCTION( z, num_params, dummy )                  \
BOOST_PP_IF( num_params                                                        \
           , BOOST_ACT_DETAIL_TIE_FUNCTION_TEMPLATE_HEADER                     \
           , BOOST_ACT_DETAIL_TIE_FUNCTION_NO_TEMPLATE_HEADER                  \
           )                                                                   \
           ( z, num_params )                                                   \
::boost::tuple                                                                 \
<                                                                              \
  BOOST_PP_ENUM_BINARY_PARAMS_Z( z, num_params, Param, & BOOST_PP_INTERCEPT )  \
>                                                                              \
tie( BOOST_PP_ENUM_BINARY_PARAMS_Z( z, num_params, Param, & arg ) )            \
{                                                                              \
  return ::boost::tuple                                                        \
  <                                                                            \
    BOOST_PP_ENUM_BINARY_PARAMS_Z( z, num_params, Param, & BOOST_PP_INTERCEPT )\
  >                                                                            \
  ( BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg ) );                            \
} 

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_TIE_FUNCTION
               , BOOST_PP_NIL
               )

}
}
}

#endif
