//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_UTILITY_HPP
#define BOOST_ACT_UTILITY_HPP

#include <boost/utility/enable_if.hp>
#include <boost/type_traits/is_void.hpp>

namespace boost
{
namespace act
{

template< typename ResultType, typename FunctionType >
typename ::boost::enable_if
<
  ::boost::is_void< ResultType >
, ResultType
>
::type
apply_function( FunctionType& function )
{
  function();
}

template< typename ResultType, typename FunctionType >
typename ::boost::disable_if
<
  ::boost::is_void< ResultType >
, ResultType
>
::type
apply_function( FunctionType& function )
{
  return static_cast< ResultType >( function() );
}

template< typename ResultType, typename FunctionType, typename TargetType >
typename ::boost::enable_if
<
  ::boost::is_void< ResultType >
, ResultType
>
::type
apply_queued_function( FunctionType& function, TargetType& target )
{
  function( target );
}

template< typename ResultType, typename FunctionType, typename TargetType >
typename ::boost::disable_if
<
  ::boost::is_void< ResultType >
, ResultType
>
::type
apply_queued_function( FunctionType& function, TargetType& target )
{
  return static_cast< ResultType >( function( target ) );
}

}
}

#endif
