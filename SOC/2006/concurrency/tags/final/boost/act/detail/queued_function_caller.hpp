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

#ifndef BOOST_ACT_DETAIL_QUEUED_FUNCTION_CALLER_HPP
#define BOOST_ACT_DETAIL_QUEUED_FUNCTION_CALLER_HPP

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "is_tuplen.hpp"
#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Add enabler
#define BOOST_ACT_DETAIL_TUPLE_ELEM( z, index, tuple_object )                  \
tuple_object.template get< index >() 

#define BOOST_ACT_DETAIL_QUEUED_FUNCTION_CALLER_FUNCTION(z,num_params,dummy)   \
template< typename ResultType                                                  \
        , typename ObjectType                                                  \
        , typename FunctionType                                                \
        , typename TupleType                                                   \
        >                                                                      \
typename ::boost::enable_if                                                    \
<                                                                              \
  ::boost::mpl::and_                                                           \
  <                                                                            \
    ::boost::mpl::not_< ::boost::is_void< ResultType > >                       \
  , BOOST_PP_CAT( is_tuple, num_params )< TupleType >                          \
  >                                                                            \
, ResultType                                                                   \
>                                                                              \
::type                                                                         \
queued_function_caller                                                         \
(                                                                              \
  ObjectType* const inactive_this                                              \
, FunctionType& function                                                       \
, TupleType const& args                                                        \
)                                                                              \
{                                                                              \
  return function( inactive_this                                               \
                   BOOST_PP_CAT( BOOST_PP_ENUM_TRAILING_, z )                  \
                   (                                                           \
                     num_params                                                \
                   , BOOST_ACT_DETAIL_TUPLE_ELEM                               \
                   , args                                                      \
                   )                                                           \
                 );                                                            \
}                                                                              \
                                                                               \
template< typename ResultType                                                  \
        , typename ObjectType                                                  \
        , typename FunctionType                                                \
        , typename TupleType                                                   \
        >                                                                      \
typename ::boost::enable_if                                                    \
<                                                                              \
  ::boost::mpl::and_                                                           \
  <                                                                            \
    ::boost::is_void< ResultType >                                             \
  , BOOST_PP_CAT( is_tuple, num_params )< TupleType >                          \
  >                                                                            \
, ResultType                                                                   \
>                                                                              \
::type                                                                         \
queued_function_caller                                                         \
(                                                                              \
  ObjectType* const inactive_this                                              \
, FunctionType& function                                                       \
, TupleType const& args                                                        \
)                                                                              \
{                                                                              \
  function( inactive_this                                                      \
            BOOST_PP_CAT( BOOST_PP_ENUM_TRAILING_, z )                         \
            (                                                                  \
              num_params                                                       \
            , BOOST_ACT_DETAIL_TUPLE_ELEM                                      \
            , args                                                             \
            )                                                                  \
          );                                                                   \
} 

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_QUEUED_FUNCTION_CALLER_FUNCTION
               , BOOST_PP_NIL
               )

#undef BOOST_ACT_DETAIL_TUPLE_ELEM

}

}
}

#endif
