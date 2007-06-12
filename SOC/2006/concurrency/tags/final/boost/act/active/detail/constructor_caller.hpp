//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_CALLER_HPP
#define BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_CALLER_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <new>

#include "../../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct constructor_caller
{
  void operator ()() const
  {
  }

  void operator ()( void* target ) const
  {
    new( target ) Type;
  }

#define BOOST_ACT_DETAIL_CONSTRUCTOR_CALLER_FUN( z, num_params, dummy )        \
  template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) >          \
  void operator ()( void* target                                               \
                    BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params      \
                                                          , Param, & arg       \
                                                          )                    \
                  ) const                                                      \
  {                                                                            \
    new( target ) Type( BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg ) );        \
  }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS )
                       , BOOST_ACT_DETAIL_CONSTRUCTOR_CALLER_FUN
                       , BOOST_PP_NIL
                       )

#undef BOOST_ACT_DETAIL_CONSTRUCTOR_CALLER_FUN

};

}

}
}

#endif
