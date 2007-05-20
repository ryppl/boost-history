/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_COMPARE_AND_ASSIGN_CAS_RELEASE_HPP
#define BOOST_ACT_INTERLOCKED_COMPARE_AND_ASSIGN_CAS_RELEASE_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

// ToDo: Change to a leaner include file
#include <windows.h>

// ToDo: Cast on interlocked bool

#if WINVER >= 0x0600


#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/integer/detail/convert_interlocked_bool_operand_to_bool.hpp>

#include <boost/act/detail/is_nonfunction_pointer.hpp>

#include <boost/detail/interlocked.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace act { namespace interlocked {

// Note: ::boost::mpl::and_ has 5 params max by default
template< typename TargetType, typename SourceType, typename ComparisonType >
typename enable_if
<
  mpl::and_
  <
    mpl::and_< has_interlocked_operations< TargetType >
             , is_integral< TargetType >
             , mpl::bool_< ( sizeof( TargetType ) == 4 ) >
             , mpl::not_< is_const< TargetType > >
             >
  , is_convertible< SourceType const, TargetType >
  , is_convertible< ComparisonType const, TargetType >
  >
, TargetType
>
::type
assign_if_was_release( TargetType volatile& destination
                     , SourceType const& new_value
                     , ComparisonType const& old_value
                     )
{
  return static_cast< TargetType >
         (
           InterlockedCompareExchangeRelease
           ( &reinterpret_cast< LONG volatile& >( destination )
           , static_cast< TargetType >( new_value )
           , static_cast< TargetType >( old_value )
           )
         );
}

template< typename TargetType, typename SourceType, typename ComparisonType >
typename enable_if
<
  mpl::and_
  <
    mpl::and_< has_interlocked_operations< TargetType >
             , act::detail::is_nonfunction_pointer< TargetType >
             , mpl::not_< is_const< TargetType > >
             >
  , is_convertible< SourceType const, TargetType >
  , is_convertible< ComparisonType const, TargetType >
  >
, TargetType
>
::type
assign_if_was_release( TargetType volatile& destination
                     , SourceType const& new_value
                     , ComparisonType const& old_value
                     )
{
  return static_cast< Type >
         (
           InterlockedCompareExchangePointerRelease
           ( &destination
           , static_cast< TargetType >( new_value )
           , static_cast< TargetType >( old_value )
           )
         );
}

template< typename TargetType, typename SourceType, typename ComparisonType >
typename enable_if
<
  mpl::and_
  <
    mpl::and_< has_interlocked_operations< TargetType >
             , detail::is_interlocked_bool< TargetType >
             , mpl::not_< is_const< TargetType > >
             >
  , mpl::or_< is_convertible< SourceType const, bool >
            , is_convertible< SourceType const, TargetType >
            >
  , mpl::or_< is_convertible< ComparisonType const, bool >
            , is_convertible< ComparisonType const, TargetType >
            >
  >
, TargetType
>
::type
assign_if_was_release( TargetType volatile& destination
                     , SourceType const& new_value
                     , ComparisonType const& old_value
                     )
{
  return assign_if_was
         ( interlocked_bool_internal_address( destination )
         , detail::convert_interlocked_bool_operand_to_bool< TargetType >
             ( new_value )
         , detail::convert_interlocked_bool_operand_to_bool< TargetType >
             ( old_value )
         );
}

} } }

#else

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>
#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType, typename ComparisonType >
typename enable_if
<
  mpl::and_
  <
    mpl::and_< has_interlocked_operations< TargetType >
             , is_integral< TargetType >
             , mpl::bool_< ( sizeof( TargetType ) == 4 ) >
             , mpl::not_< is_const< TargetType > >
             >
  , mpl::or_
    <
      mpl::and_< detail::is_interlocked_bool< TargetType >
               , is_convertible< SourceType const, bool >
               >
    , is_convertible< SourceType const, TargetType >
    >
  , mpl::or_
    <
      mpl::and_< detail::is_interlocked_bool< TargetType >
               , is_convertible< ComparisonType const, bool >
               >
    , is_convertible< ComparisonType const, TargetType >
    >
  >
, TargetType
>
::type
assign_if_was_release( TargetType volatile& destination
                     , SourceType const& new_value
                     , ComparisonType const& old_value
                     )
{
  return assign_if_was( destination, new_value, old_value );
}

} } }

#endif

#endif

#endif
