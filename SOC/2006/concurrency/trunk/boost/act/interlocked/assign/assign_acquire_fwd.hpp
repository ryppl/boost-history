/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_ACQUIRE_FWD_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_ACQUIRE_FWD_HPP

#include <boost/utility/enable_if.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/assign/assign_result.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

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
assign_acquire( TargetType& destination, SourceType const& new_value );

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
assign_acquire( TargetType& destination, SourceType const& new_value );

} } }

#endif
