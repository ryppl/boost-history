/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_STORE_STORE_RELEASE_FWD_HPP
#define BOOST_ACT_INTERLOCKED_STORE_STORE_RELEASE_FWD_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( store, release )

#include <boost/act/interlocked/semantics/release.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics, typename TargetType, typename SourceType >
typename enable_if
<
  mpl::and_
  <
    is_same< Semantics, release >
  , detail::are_valid_store_style_params< TargetType, SourceType const >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
>
::type
store( TargetType& destination, SourceType const& new_value );

template< typename Semantics, typename TargetType, typename SourceType >
typename enable_if
<
  mpl::and_
  <
    is_same< Semantics, release >
  , detail::are_valid_store_style_params< TargetType, SourceType const >
  , detail::is_interlocked_bool< TargetType >
  >
>
::type
store( TargetType& destination, SourceType const& new_value );

} } }

#endif

#endif
