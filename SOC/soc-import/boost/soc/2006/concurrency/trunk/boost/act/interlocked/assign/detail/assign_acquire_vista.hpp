#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_VISTA_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_VISTA_HPP

#include <boost/config.hpp>

#ifndef BOOST_WINDOWS
#error Windows interlocked assign acquire included on pre-vista Windows OS.
#else

#include <windows.h>

#if WINVER < 0x0600
#error Windows interlocked assign acquire included on pre-vista Windows OS.
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

#include <boost/act/interlocked/assign/detail/assign_windows.hpp>
#include <boost/act/interlocked/detail/interlocked_result.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

namespace assign {

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , is_integral< TargetType >
           , bool_< ( sizeof( Type ) == 4 ) >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           >
, assign_result< TargetType, SourceType >
>
::type
assign_acquire( TargetType volatile& destination
              , SourceType const& new_value
              )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
             InterlockedExchangeAcquire
             ( &reinterpret_cast< LONG volatile& >( destination )
             , new_value
             )
           )
         , new_value
         );
}

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , is_integral< TargetType >
           , bool_< ( sizeof( Type ) == 8 ) >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           >
, assign_result< TargetType, SourceType >
>
::type
assign_acquire( TargetType volatile& destination
              , SourceType const& new_value
              )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
             InterlockedExchange64Acquire
             ( &reinterpret_cast< LONGLONG volatile& >( destination )
             , new_value
             )
           )
         , new_value
         );
}


template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_< has_interlocked_operations< TargetType >
           , act::detail::is_nonfunction_pointer< TargetType >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           >
, assign_result< TargetType, SourceType >
>
::type
assign_acquire( TargetType volatile& destination
              , SourceType const& new_value
              )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
             InterlockedExchangePointerAcquire( &destination
                                              , new_value
                                              )
           )
         , new_value
         );
}

template< typename TargetType, typename SourceType >
typename lazy_enable_if
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
assign_acquire( TargetType volatile& destination
              , SourceType const& new_value
              )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
                InterlockedExchangeAcquire
                ( interlocked_bool_internal_address( destination )
                , detail::convert_interlocked_bool_operand_to_bool< TargetType >
                    ( new_value )
                )
             != 0
           )
         , new_value
         );
}

}

} } } }

#endif

#endif
