#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_WINDOWS_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_WINDOWS_HPP

#include <boost/config.hpp>

#ifndef BOOST_WINDOWS
#error Windows interlocked assign included on non-windows OS.
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

#include <boost/act/interlocked/detail/interlocked_result.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
struct assign_result
{
  typedef detail::binary_interlocked_result< detail::assign_operation
                                           , detail::old_value
                                           , TargetType, SourceType
                                           >
          type;
};

namespace detail { namespace assign {

template< typename TargetType, typename SourceType >
struct are_valid_arithmetic32_params
  : mpl::and_< has_interlocked_operations< TargetType >
             , is_integral< TargetType >
             , mpl::bool_< ( sizeof( TargetType ) == 4 ) >
             , mpl::not_< is_const< TargetType > >
             , is_convertible< SourceType const, TargetType >
             > {};

#if WINVER >= 0x0600

template< typename TargetType, typename SourceType >
struct are_valid_arithmetic64_params
  : mpl::and_< has_interlocked_operations< TargetType >
             , is_integral< TargetType >
             , mpl::bool_< ( sizeof( TargetType ) == 4 ) >
             , mpl::not_< is_const< TargetType > >
             , is_convertible< SourceType const, TargetType >
             > {};

template< typename TargetType, typename SourceType >
struct are_valid_arithmetic_params
  : mpl::or_< are_valid_arithmetic32_params< TargetType, SourceType >
            , are_valid_arithmetic64_params< TargetType, SourceType >
            > {};

#else

template< typename TargetType, typename SourceType >
struct are_valid_arithmetic_params
  : are_valid_arithmetic32_params< TargetType, SourceType > {};

#endif

template< typename TargetType, typename SourceType >
struct are_valid_pointer_params
  : mpl::and_< has_interlocked_operations< TargetType >
           , act::detail::is_nonfunction_pointer< TargetType >
           , mpl::not_< is_const< TargetType > >
           , is_convertible< SourceType const, TargetType >
           > {};

template< typename TargetType, typename SourceType >
struct are_valid_bool_params
  : mpl::and_< has_interlocked_operations< TargetType >
             , detail::is_interlocked_bool< TargetType >
             , mpl::not_< is_const< TargetType > >
             , mpl::or_< is_convertible< SourceType const, bool >
                       , is_convertible< SourceType const, TargetType >
                       >
             > {};

template< typename TargetType, typename SourceType >
struct are_valid_params
  : mpl::or_< are_valid_arithmetic_params< TargetType, SourceType >
            , are_valid_pointer_params< TargetType, SourceType >
            , are_valid_bool_params< TargetType, SourceType >
            >
{
};

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  are_valid_arithmetic32_params< TargetType, SourceType >
, assign_result< TargetType, SourceType >
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
             BOOST_INTERLOCKED_EXCHANGE
             ( &reinterpret_cast< LONG volatile& >( destination )
             , static_cast< LONG >( new_value )
             )
           )
         , new_value
         );
}

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  are_valid_pointer_params< TargetType, SourceType >
, assign_result< TargetType, SourceType >
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
             BOOST_INTERLOCKED_EXCHANGE_POINTER
             ( &destination
             , reinterpret_cast< void* >( new_value )
             )
           )
         , new_value
         );
}

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  are_valid_bool_params< TargetType, SourceType >
, assign_result< TargetType, SourceType >
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
                assign
                ( interlocked_bool_internal_value( destination )
                , detail::convert_interlocked_bool_operand_to_bool< TargetType >
                    ( new_value )
                )
                .old_value()
             != 0
           )
         , new_value
         );
}

#if WINVER >= 0x0600

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  are_valid_arithmetic64_params< TargetType, SourceType >
, assign_result< TargetType, SourceType >
>
::type
assign( TargetType volatile& destination, SourceType const& new_value )
{
  return make_interlocked_result< assign_operation, old_value >
         (
           static_cast< TargetType >
           (
             InterlockedExchange64
             ( &reinterpret_cast< LONGLONG volatile& >( destination )
             , static_cast< LONGLONG >( new_value )
             )
           )
         , new_value
         );
}

#endif

} }

} } }

#endif

#endif
