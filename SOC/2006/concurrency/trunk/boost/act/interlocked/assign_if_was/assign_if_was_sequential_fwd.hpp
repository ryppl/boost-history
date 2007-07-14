/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_ASSIGN_IF_WAS_SEQ_FWD_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_ASSIGN_IF_WAS_SEQ_FWD_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_INTERLOCKED_HAS( assign_if_was, sequential )

#include <boost/act/interlocked/semantics/sequential.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics
        , typename TargetType, typename SourceType, typename ConditionType
        >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, sequential >
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
             );

template< typename Semantics
        , typename TargetType, typename SourceType, typename ConditionType
        >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, sequential >
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
             );

} } }

#endif

#endif
