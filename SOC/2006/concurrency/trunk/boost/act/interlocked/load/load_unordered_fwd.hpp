/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_LOAD_LOAD_UNORDERED_FWD_HPP
#define BOOST_ACT_INTERLOCKED_LOAD_LOAD_UNORDERED_FWD_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( load, unordered )

#include <boost/act/interlocked/semantics/unordered.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics, typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, unordered >
  , detail::are_valid_store_style_params< TargetType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
load( TargetType const& target );

template< typename Semantics, typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, unordered >
  , detail::are_valid_store_style_params< TargetType >
  , detail::is_interlocked_bool< TargetType >
  >
, remove_cv< TargetType >
>
::type
load( TargetType& destination );

} } }

#endif

#endif
