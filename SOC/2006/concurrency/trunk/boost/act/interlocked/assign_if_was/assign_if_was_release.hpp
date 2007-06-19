/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_ASSIGN_IF_WAS_RELEASE_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_ASSIGN_IF_WAS_RELEASE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/assign_if_was/assign_if_was_result.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO                                 \
( assign_if_was, release )

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType, typename ConditionType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType, SourceType const
                                         , ConditionType const
                                         >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, assign_if_was_result< TargetType >
>
::type
assign_if_was_release( TargetType& destination, SourceType const& new_value
                     , ConditionType const& expected_value
                     )
{
  typedef typename remove_volatile< TargetType >::type type;

  type const source    = static_cast< type >( new_value ),
             old_value = static_cast< type >( expected_value );

  typedef typename assign_if_was_result< TargetType >::type result_type;

  return detail::assign_if_was_release_impl
         ::execute( destination, source, old_value );
}

template< typename TargetType, typename SourceType, typename ConditionType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType, SourceType const
                                         , ConditionType const
                                         >
  , detail::is_interlocked_bool< TargetType >
  >
, assign_if_was_result< TargetType >
>
::type
assign_if_was_release( TargetType& destination, SourceType const& new_value
                     , ConditionType const& expected_value
                     )
{
  typedef typename assign_if_was_result< TargetType >::type result_type;

  typedef typename TargetType::internal_type internal_type;

  return result_type
         (
           interlocked::assign_if_was_release
           ( interlocked_bool_internal_value( destination )
           , static_cast< bool >( new_value )
           , static_cast< bool >( expected_value )
           )
           .old_value() != 0
         , static_cast< bool >( new_value )
         , static_cast< bool >( expected_value )
         );
}

} } }

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_END()

#endif
