/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_LOAD_LOAD_ACQUIRE_HPP
#define BOOST_ACT_INTERLOCKED_LOAD_LOAD_ACQUIRE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>

#if BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_LOAD_ACQUIRE

#include <boost/act/interlocked/load/detail/load_acquire_default>

namespace boost { namespace act { namespace interlocked { namespace detail {

struct load_acquire_impl : load_acquire_default_impl {};

} } } }

#else


#endif

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  detail::are_valid_store_style_params< TargetType >
, remove_volatile< TargetType >
>
::type
load_acquire( TargetType const& target )
{
  return detail::load_acquire_impl::execute( target );
}

} } }

#endif
