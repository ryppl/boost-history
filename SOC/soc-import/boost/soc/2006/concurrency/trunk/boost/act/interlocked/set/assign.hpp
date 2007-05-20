/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_HPP

#include <boost/config.hpp>

#ifndef BOOST_WINDOWS
#error Interlocked operations not supported
#else

#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/integer/detail/convert_interlocked_bool_operand_to_bool.hpp>

#include <boost/detail/interlocked.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/act/detail/is_nonfunction_pointer.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <windows.h> // ToDo: Remove (forward declare functions)

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
typename enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , is_integral< TargetType >
           , mpl::bool_< ( sizeof( TargetType ) == 4 ) >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           >
, TargetType
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return static_cast< TargetType >
         (
           BOOST_INTERLOCKED_EXCHANGE
           ( &reinterpret_cast< LONG volatile& >( destination )
           , static_cast< LONG >( new_value )
           )
         );
}

template< typename TargetType, typename SourceType >
typename enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , act::detail::is_nonfunction_pointer< TargetType >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           >
, TargetType
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return static_cast< TargetType >
         (
           BOOST_INTERLOCKED_EXCHANGE_POINTER
           ( &destination
           , reinterpret_cast< void* >( new_value )
           )
         );
}

template< typename TargetType, typename SourceType >
typename enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , detail::is_interlocked_bool< TargetType >
           , mpl::not_< is_const< TargetType > >
           , mpl::or_< is_convertible< SourceType const, bool >
                     , is_convertible< SourceType const, TargetType >
                     >
           >
, TargetType
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return    assign
            ( interlocked_bool_internal_value( destination )
            , detail::convert_interlocked_bool_operand_to_bool< TargetType >
                ( new_value )
            )
         != 0;
}

#if WINVER >= 0x0600

template< typename TargetType, typename SourceType >
typename enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , is_integral< TargetType >
           , mpl::bool_< ( sizeof( Type ) == 8 ) >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           >
, Type
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return static_cast< TargetType >
         (
           InterlockedExchange64
           ( &reinterpret_cast< LONGLONG volatile& >( destination )
           , static_cast< LONGLONG >( new_value )
           )
         );
}

#endif

} } }

#endif

#endif
