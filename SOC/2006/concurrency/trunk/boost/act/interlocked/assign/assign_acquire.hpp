/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_ACQUIRE_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_ACQUIRE_HPP

#include <boost/act/interlocked/assign/detail/assign_acquire_impl.hpp>
#include <boost/act/interlocked/assign/detail/assign_acquire_default_impl.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/assign/assign_result.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl_decl.hpp>
#include <boost/act/interlocked/detail/has_nested_type.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType, SourceType const >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, assign_result< TargetType >
>
::type
assign_acquire( TargetType& destination, SourceType const& new_value )
{
  typedef typename remove_volatile< TargetType >::type type;
  type const source = static_cast< type >( new_value );
  typedef typename assign_result< TargetType >::type result_type;

  typedef detail::assign_acquire_impl
          < result_type
          , type
          >
          impl_type;

  return mpl::if_< detail::has_nested_type< impl_type >
                 , impl_type
                 , detail::assign_acquire_default_impl< result_type, type >
                 >
                 ::type
                 ::execute( destination, source );

}

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType, SourceType const >
  , detail::is_interlocked_bool< TargetType >
  >
, assign_result< TargetType >
>
::type
assign_acquire( TargetType& destination, SourceType const& new_value )
{
  typedef typename assign_result< TargetType >::type result_type;

  typedef typename TargetType::internal_type internal_type;

  return result_type
         (
           interlocked::assign_acquire
           ( interlocked_bool_internal_value( destination )
           , static_cast< bool >( new_value )
           )
           .old_value() != 0
         , static_cast< bool >( new_value )
         );
}

} } }

#endif
