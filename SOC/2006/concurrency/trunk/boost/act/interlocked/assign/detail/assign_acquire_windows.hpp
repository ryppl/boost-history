/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_VISTA_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_VISTA_HPP

#include <boost/config.hpp>

#ifndef BOOST_WINDOWS
#error Windows interlocked assign acquire included on non-Windows OS.
#else

#include <windows.h>

#if WINVER < 0x0600
#error Windows interlocked assign acquire included on pre-vista Windows OS.
#else

#include <boost/utility/enable_if.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct assign_acquire_impl
{
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
               InterlockedExchangeAcquire
               ( reinterpret_cast< LONG volatile* >( &left )
               , reinterpret_cast< LONG const& >( right )
               )
             )
           , right
           );
  }

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
               InterlockedExchange64Acquire
               ( reinterpret_cast< LONGLONG volatile* >( &left )
               , reinterpret_cast< LONGLONG const& >( right )
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
               InterlockedExchangePointerAcquire
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
};

} } } }

#endif

#endif
