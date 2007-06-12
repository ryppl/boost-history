/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DECREMENT_DEC_HPP
#define BOOST_ACT_INTERLOCKED_DECREMENT_DEC_HPP

#include <boost/config.hpp>

#ifndef BOOST_WINDOWS
#error Interlocked operations not supported
#else

#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>

#include <boost/act/detail/is_pointer_to_void.hpp>

#include <boost/detail/interlocked.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_const.hpp>

#include <windows.h> // ToDo: Remove (forward declare functions)

namespace boost { namespace act { namespace interlocked {

template< typename Type >
typename enable_if
<
  mpl::and_< has_interlocked_operations< Type >
           , is_integral< Type >
           , mpl::not_< is_same< Type, bool > >
           , mpl::bool_< ( sizeof( Type ) == 4 ) >
           , mpl::not_< is_const< Type > >
           >
, Type
>
::type
decrement( Type volatile& destination )
{
  return BOOST_INTERLOCKED_DECREMENT
         ( &reinterpret_cast< LONG volatile& >( destination ) );
}

template< typename Type >
typename enable_if
<
  mpl::and_< has_interlocked_operations< Type >
           , is_pointer< Type >
           , mpl::not_< act::detail::is_pointer_to_void< Type > >
           , mpl::not_< is_const< Type > >
           >
, Type
>
::type
decrement( Type volatile& destination )
{
  Type expected_old_value = destination,
       old_value;

  do
  {
    old_value = assign_if_was( destination
                               , expected_old_value - 1
                               , expected_old_value
                               )

    if( old_value == expected_old_value )
      break;
    else
      expected_old_value = old_value;
  }
  while( true );

  return old_value;
}

#if WINVER >= 0x0600

template< typename Type >
typename enable_if
<
  mpl::and_< has_interlocked_operations< Type >
           , is_integral< Type >
           , mpl::not_< is_same< Type, bool > >
           , mpl::bool_< ( sizeof( Type ) == 8 ) >
           , mpl::not_< is_const< Type > >
           >
, Type
>
::type
decrement( Type volatile& destination )
{
  return InterlockedDecrement64
         ( &reinterpret_cast< LONGLONG volatile& >( destination ) );
}

#endif

} } }

#endif

#endif
