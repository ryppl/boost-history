/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_WINDOWS_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_WINDOWS_HPP

#include <boost/config.hpp>

#ifndef BOOST_WINDOWS
#error Windows interlocked assign included on non-windows OS.
#else

#include <boost/detail/interlocked.hpp>

#include <boost/utility/enable_if.hpp>

#include <windows.h> // ToDo: Remove (forward declare functions)

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct assign_impl
{
  typedef void type;

  template< typename LeftType, typename RightType >
  static
  typename enable_if_c
  <
    ( sizeof( LeftType ) == 4 )
  , ResultType
  >
  ::type
  execute( LeftType& left, RightType& right )
  {
    return ResultType
           (
             static_cast< UnqualifiedType >
             (
               BOOST_INTERLOCKED_EXCHANGE
               ( reinterpret_cast< LONG volatile* >( &left )
               , reinterpret_cast< LONG const& >( right )
               )
             )
           , right
           );
  }

  template< typename LeftType, typename RightType >
  static
  ResultType
  execute( LeftType*& left, RightType*& right )
  {
    return ResultType
           (
             static_cast< UnqualifiedType >
             (
               BOOST_INTERLOCKED_EXCHANGE_POINTER
               ( const_cast< void* volatile* >
                 (
                   reinterpret_cast< void const volatile* volatile* >( &left )
                 )
               , const_cast< void* >( right )
               )
             )
           , right
           );
    
  }

#if WINVER >= 0x0600

  template< typename LeftType, typename RightType >
  static
  typename enable_if_c
  <
    ( sizeof( LeftType ) == 8 )
  , ResultType
  >
  ::type
  execute( LeftType& left, RightType& right )
  {
    return ResultType
           (
             static_cast< UnqualifiedType >
             (
               InterlockedExchange64
               ( reinterpret_cast< LONGLONG volatile* >( &left )
               , reinterpret_cast< LONGLONG const& >( right )
               )
             )
           , right
           );
  }

#endif

};

} } } }

#endif

#endif
