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
#include <boost/act/interlocked/detail/interlocked_result.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct assign_acquire_impl
{
  template< typename TargetType, typename OperandType >
  struct result
    : binary_interlocked_result_returns_old< assign_operation
                                           , TargetType, OperandType
                                           > {};

  template< typename LeftType, typename RightType >
  static
  typename lazy_enable_if_c
  <
    ( sizeof( LeftType ) == 4 )
  , result< LeftType, RightType >
  >
  ::type
  execute( LeftType& left, RightType& right )
  {
    typedef typename result< LeftType, RightType >::type result_type;
    typedef typename remove_cv< LeftType >::type unqualified_type;

    return result_type
           (
             static_cast< unqualified_type >
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
  typename lazy_enable_if_c
  <
    ( sizeof( LeftType ) == 8 )
  , result< LeftType, RightType >
  >
  ::type
  execute( LeftType& left, RightType& right )
  {
    typedef typename result< LeftType, RightType >::type result_type;
    typedef typename remove_cv< LeftType >::type unqualified_type;

    return result_type
           (
             static_cast< unqualified_type >
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
  static typename result< LeftType, RightType >
  execute( LeftType*& left, RightType*& right )
  {
    typedef typename result< LeftType*, RightType >::type result_type;
    typedef typename remove_cv< LeftType* >::type unqualified_type;

    return ResultType
           (
             static_cast< unqualified_type >
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
