/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_STORE_STORE_HPP
#define BOOST_ACT_INTERLOCKED_STORE_STORE_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( store, release )

#include <boost/utility/enable_if.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/store/store_release.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
typename enable_if
<
  detail::are_valid_store_style_params< TargetType, SourceType const >
>
::type
store( TargetType& destination, SourceType const& new_value )
{
  store< release >( destination, new_value );
}

} } }

#endif

#endif
