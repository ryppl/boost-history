/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_RETRIEVE_HPP
#define BOOST_ACT_INTERLOCKED_RETRIEVE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>

#if BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_RETRIEVE

#include <boost/act/interlocked/retrieve/detail/retrieve_default.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct retrieve_impl : retrieve_default_impl< ResultType, UnqualifiedType > {};

} } } }

#else

#error Custom retrieve functionality is not yet implemented

#endif

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  detail::are_valid_assign_style_params< TargetType >
, remove_volatile< TargetType >
>
::type
retrieve( TargetType const& target )
{
  typedef typename remove_volatile< TargetType >::type unqualified_type;

  return detail::retrieve_impl< unqualified_type, unqualified_type >
         ::execute( target );
}

} } }

#endif
