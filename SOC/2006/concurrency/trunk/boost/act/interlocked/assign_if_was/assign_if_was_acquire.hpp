/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_ASSIGN_IF_WAS_ACQUIRE_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_ASSIGN_IF_WAS_ACQUIRE_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_INTERLOCKED_HAS( assign_if_was, acquire )

#include <boost/act/interlocked/semantics/acquire.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/integer/selection.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/assign_if_was/detail/unaligned_assign_if_was.hpp>

#include <boost/act/interlocked/detail/impl.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO                                 \
( assign_if_was, acquire )

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()

namespace boost { namespace act { namespace interlocked {

namespace detail
{
template< typename Semantics >
struct unaligned_assign_if_was;
}

template< typename Semantics
        , typename TargetType, typename SourceType, typename ConditionType
        >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, acquire >
  , detail::are_valid_store_style_params< TargetType, SourceType const
                                         , ConditionType const
                                         >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
assign_if_was( TargetType& destination, SourceType const& new_value
             , ConditionType const& expected_value
             )
{
  return mpl::if_< detail::is_unaligned_interlocked< TargetType >
                 , detail::unaligned_assign_if_was< Semantics >
                 , detail::assign_if_was_acquire_impl
                 >
                 ::type::execute( destination, new_value, expected_value );
}

template< typename Semantics
        , typename TargetType, typename SourceType, typename ConditionType
        >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, acquire >
  , detail::are_valid_store_style_params< TargetType, SourceType const
                                         , ConditionType const
                                         >
  , detail::is_interlocked_bool< TargetType >
  >
, remove_cv< TargetType >
>
::type
assign_if_was( TargetType& destination, SourceType const& new_value
             , ConditionType const& expected_value
             )
{
  typedef typename remove_cv< TargetType >::type result_type;

  return result_type
         (
           interlocked::assign_if_was< acquire >
           ( interlocked_bool_internal_value( destination )
           , static_cast< bool >( new_value )
           , static_cast< bool >( expected_value )
           )
           != 0
         );
}

} } }

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_END()

#endif

#endif
