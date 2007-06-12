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

#ifndef BOOST_ACT_ACTIVE_QUEUE_FUNCTION_HPP
#define BOOST_ACT_ACTIVE_QUEUE_FUNCTION_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "../act_fwd.hpp"
#include "../detail/detail_fwd.hpp"
#include "../detail/tie.hpp"

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
/*
#define BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNC( z, num_params, dummy )             \
template< typename ActiveObjectType                                            \
        , typename ActableFunctionType                                         \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )     \
        >                                                                      \
typename detail::queue_function_enabler                                        \
<                                                                              \
  ActiveObjectType                                                             \
, ActableFunctionType                                                          \
, ::boost::mpl::deque                                                          \
    < BOOST_PP_ENUM_BINARY_PARAMS_Z(z,num_params,Param,&BOOST_PP_INTERCEPT) >  \
>                                                                              \
::type                                                                         \
queue_function                                                                 \
(                                                                              \
  ActiveObjectType& object                                                     \
, ActableFunctionType const& function                                          \
  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, & arg )        \
)                                                                              \
{                                                                              \
  typedef typename ActiveObjectType::act_model act_model;                      \
                                                                               \
  return action< typename ActableFunctionType::logical_result_type             \
               , act_model                                                     \
               >                                                               \
         (                                                                     \
           typename act_model::action_constructor_queued_call_tag()            \
         , object.impl()                                                       \
         , function                                                            \
         , detail::tie                                                         \
           (                                                                   \
             BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg )                      \
           )                                                                   \
         );                                                                    \
}                                                                              \
                                                                               \
template< typename ActiveObjectType                                            \
        , typename ActableFunctionType                                         \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )     \
        >                                                                      \
typename detail::queue_function_disabler                                       \
<                                                                              \
  ActiveObjectType                                                             \
, ActableFunctionType                                                          \
, ::boost::mpl::deque                                                          \
    < BOOST_PP_ENUM_BINARY_PARAMS_Z(z,num_params,Param,&BOOST_PP_INTERCEPT) >  \
>                                                                              \
::type                                                                         \
queue_function                                                                 \
(                                                                              \
  ActiveObjectType& object                                                     \
, ActableFunctionType const& function                                          \
  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, & arg )        \
)                                                                              \
{                                                                              \
  BOOST_MPL_ASSERT_MSG( false, INVALID_QUEUE_FUNCTION_PARAMS, () );            \
} 

BOOST_PP_REPEAT( BOOST_ACT_MAX_PARAMS
               , BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNC
               , BOOST_PP_NIL
               )
*/
}
}

#include "../type_traits/is_act_model.hpp"

#include "../detail/active_call_enablers.hpp"

#include "../config/default_act_model.hpp"

#endif
