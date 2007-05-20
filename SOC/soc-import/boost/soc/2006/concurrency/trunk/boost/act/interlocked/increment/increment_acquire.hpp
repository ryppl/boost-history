/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INCREMENT_INC_ACQUIRE_HPP
#define BOOST_ACT_INTERLOCKED_INCREMENT_INC_ACQUIRE_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

// ToDo: Change to a leaner include file
#include <windows.h>

// ToDo: Cast on interlocked bool

#if WINVER >= 0x0600

#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/assign_if_was/assign_if_was_acquire.hpp>

#include <boost/act/detail/is_pointer_to_void.hpp>

#include <boost/detail/interlocked.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type >
typename enable_if
<
  mpl::and_< has_interlocked_operations< Type >
           , is_integral< Type >
           , mpl::not_< is_same< Type, bool > >
           , bool_< ( sizeof( Type ) == 4 ) >
           , mpl::not_< is_const< Type > >
           >
, Type
>
::type
increment_acquire( Type volatile& destination )
{
  return static_cast< Type >
         (
           InterlockedIncrementAcquire
           ( &reinterpret_cast< LONG volatile& >( destination ) )
         );
}

template< typename Type >
typename enable_if
<
  mpl::and_< has_interlocked_operations< Type >
           , is_integral< Type >
           , mpl::not_< is_same< Type, bool > >
           , bool_< ( sizeof( Type ) == 8 ) >
           , mpl::not_< is_const< Type > >
           >
, Type
>
::type
increment_acquire( Type volatile& destination )
{
  return static_cast< Type >
         (
           InterlockedIncrement64Acquire
           ( &reinterpret_cast< LONGLONG volatile& >( destination ) )
         );
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
increment_acquire( Type volatile& destination )
{
  Type expected_value;

  do
  {
    expected_value = destination;
  }
  while(    assign_if_was_acqire( destination
                                  , expected_value + 1
                                  , expected_value
                                  )
         != expected_value
       );

  return expected_value;
}

} } }

#else

#include <boost/act/interlocked/increment/increment.hpp>
#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type >
typename enable_if
<
  mpl::and_
  <
    has_interlocked_operations< Type >
  , mpl::not_< is_same< Type, bool > >
  , mpl::not_< detail::is_interlocked_bool< Type > >
  , mpl::not_< act::detail::is_pointer_to_void< Type > >
  , mpl::not_< is_const< Type > >
  >
, Type
>
::type
increment_acquire( Type volatile& destination )
{
  return increment( destination );
}

} } }

#endif

#endif

#endif
