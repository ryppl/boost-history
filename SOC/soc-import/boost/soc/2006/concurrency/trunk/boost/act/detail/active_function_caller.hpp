//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_FUNCTION_CALLER_HPP
#define BOOST_ACT_DETAIL_ACTIVE_FUNCTION_CALLER_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include "../config/max_active_call_params.hpp"
#include "is_tuplen.hpp"

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Add enabler
#define BOOST_ACT_DETAIL_TUPLE_ELEM( z, index, tuple_object )                  \
tuple_object.template get< index >() 

#define BOOST_ACT_DETAIL_ACTIVE_FUNCTION_CALLER_FUNCTION(z,num_params,dummy)   \
template< typename ResultType                                                  \
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
active_function_caller                                                         \
(                                                                              \
  FunctionType& function                                                       \
, TupleType const& args                                                        \
)                                                                              \
{                                                                              \
  return function( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                           \
                   (                                                           \
                     num_params                                                \
                   , BOOST_ACT_DETAIL_TUPLE_ELEM                               \
                   , args                                                      \
                   )                                                           \
                 );                                                            \
}                                                                              \
                                                                               \
template< typename ResultType                                                  \
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
active_function_caller                                                         \
(                                                                              \
  FunctionType& function                                                       \
, TupleType const& args                                                        \
)                                                                              \
{                                                                              \
  function( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                                  \
            (                                                                  \
              num_params                                                       \
            , BOOST_ACT_DETAIL_TUPLE_ELEM                                      \
            , args                                                             \
            )                                                                  \
          );                                                                   \
} 

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_ACTIVE_FUNCTION_CALLER_FUNCTION
               , BOOST_PP_NIL
               )

#undef BOOST_ACT_DETAIL_TUPLE_ELEM

}

}
}

#endif
