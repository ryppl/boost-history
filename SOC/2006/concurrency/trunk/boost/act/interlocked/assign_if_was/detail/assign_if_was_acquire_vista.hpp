/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_ASSIGN_IF_WAS_ACQ_VISTA_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_ASSIGN_IF_WAS_ACQ_VISTA_HPP

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
struct assign_if_was_acquire_impl
{
  typedef void type;

  template< typename LeftType, typename RightType, typename OldType >
  static
  typename enable_if_c
  <
    ( sizeof( LeftType ) == 4 )
  , ResultType
  >
  ::type
  execute( LeftType& left, RightType& right, OldType& old )
  {
    return ResultType
           (
             static_cast< UnqualifiedType >
             (
               InterlockedCompareExchangeAcquire
               ( reinterpret_cast< LONG volatile* >( &left )
               , reinterpret_cast< LONG const& >( right )
               )
             )
           , right
           , old
           );
  }

  template< typename LeftType, typename RightType, typename OldType >
  static
  typename enable_if_c
  <
    ( sizeof( LeftType ) == 8 )
  , ResultType
  >
  ::type
  execute( LeftType& left, RightType& right, OldType& old )
  {
    return ResultType
           (
             static_cast< UnqualifiedType >
             (
               InterlockedCompareExchange64Acquire
               ( reinterpret_cast< LONGLONG volatile* >( &left )
               , reinterpret_cast< LONGLONG const& >( right )
               , reinterpret_cast< LONGLONG const& >( old )
               )
             )
           , right
           , old
           );
  }

  template< typename LeftType, typename RightType, typename OldType >
  static
  ResultType
  execute( LeftType*& left, RightType*& right, OldType*& old )
  {
    return ResultType
           (
             static_cast< UnqualifiedType >
             (
               InterlockedCompareExchangePointerAcquire
               ( const_cast< void* volatile* >
                 (
                   reinterpret_cast< void const volatile* volatile* >( &left )
                 )
               , const_cast< void* >( right )
               , const_cast< void* >( old )
               )
             )
           , right
           , old
           );
  }
};

} } } }

#endif

#endif
