//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

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

BOOST_PP_REPEAT( BOOST_ACT_MAX_PARAMS
               , BOOST_ACT_DETAIL_TIE_FUNCTION
               , BOOST_PP_NIL
               )

}
}
}

#endif
