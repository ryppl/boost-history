/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_COMPARE_AND_ASSIGN_CAS_HPP
#define BOOST_ACT_INTERLOCKED_COMPARE_AND_ASSIGN_CAS_HPP

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
#include <boost/type_traits/is_const.hpp>

#include <boost/act/detail/is_nonfunction_pointer.hpp>

#include <windows.h> // ToDo: Remove (forward declare functions)

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
assign_if_was( TargetType volatile& destination, SourceType const& new_value
             , ComparisonType const& old_value
             )
{
  return static_cast< TargetType >
         (
           BOOST_INTERLOCKED_COMPARE_EXCHANGE
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
assign_if_was( TargetType volatile& destination, SourceType const& new_value
             , ComparisonType const& old_value
             )
{
  return static_cast< Type >
         (
           BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER
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
assign_if_was( TargetType volatile& destination, SourceType const& new_value
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

#if WINVER >= 0x0600

template< typename Type >
typename enable_if
<
  mpl::and_< has_interlocked_operations< Type >
           , is_integral< Type >
           , mpl::bool_< ( sizeof( Type ) == 8 ) >
           , mpl::not_< is_const< Type > >
           >
, Type
>
::type
assign_if_was( Type volatile& destination, Type new_value, Type old_value )
{
  return static_cast< Type >
         (
           InterlockedCompareExchange64
           ( &reinterpret_cast< LONGLONG volatile& >( destination )
           , new_value, old_value
           )
         );
}

#endif

} } }

#endif

#endif

/*

mcas implementation pseudo code

tuple< var, var2 >assign_if_was( var variable, new_val, old_val
                               , var2 variable2, new_val2, old_val2
                               )
{
  var var_val;
  var2 var_val2;

  if( old_val == ( var_val = assign_if_was( variable, new_val, old_val ) ) )
    if( old_val2 != assign_if_was( variable2, new_val2, old_val2 ) )
      
}


*/
