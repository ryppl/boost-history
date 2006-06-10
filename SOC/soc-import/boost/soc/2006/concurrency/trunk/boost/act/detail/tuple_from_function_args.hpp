//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_TUPLE_FROM_FUNCTION_ARGS_HPP
#define BOOST_ACT_DETAIL_TUPLE_FROM_FUNCTION_ARGS_HPP

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType
        , typename Enabler = void
        >
struct tuple_from_function_args;

template< typename FunctionType >
struct tuple_from_function_args
       <
         FunctionType
       , typename ::boost::enable_if
           < ::boost::is_function< FunctionType > >::type
       >
  : tuple_from_sequence< typename argument_sequence< FunctionType >::type >
{
};

template< typename ActableFunctionType >
struct tuple_from_function_args
       <
         ActableFunctionType
       , typename ::boost::enable_if
         <
           is_actable_function< ActableFunctionType >
         >
         ::type
       >
  : tuple_from_function< typename ActableFunctionType::logical_function_type >
{
};

}
}
}

#endif
