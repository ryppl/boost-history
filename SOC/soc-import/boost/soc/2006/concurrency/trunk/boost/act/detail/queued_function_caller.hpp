//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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
