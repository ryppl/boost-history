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

#ifndef BOOST_ACT_ACT_FWD_HPP
#define BOOST_ACT_ACT_FWD_HPP

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include "detail/detail_fwd.hpp"

#include "config/max_active_call_params.hpp"

#include "detail/enabler_dummy.hpp"

#include "algorithm/algorithm_fwd.hpp"

#include "default_algo_model/default_algo_model_fwd.hpp"

namespace boost
{
namespace act
{

struct act_model;

struct immediate_act_model;
struct lazy_act_model;
struct concurrent_act_model;

typedef concurrent_act_model default_act_model;

struct parallel_safe;

struct parallel_algo_model;
struct serial_algo_model;

template< typename RepresentedType = void
        , typename ActModel = default_act_model
        >
class action;

template< typename Type, typename Enabler = void >
struct associated_algo_model;

template< typename Type, typename ActiveDetails, typename Enabler = void >
class active_interface;

#define BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNCTION_DECL( z, num_params, dummy )    \
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
);                                                                             \
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
); 

BOOST_PP_REPEAT( BOOST_PP_DEC( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS )
               , BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNCTION_DECL
               , BOOST_PP_NIL
               )
}
}

#endif
