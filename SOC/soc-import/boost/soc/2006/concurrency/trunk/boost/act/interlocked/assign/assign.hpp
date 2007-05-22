/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_HPP

#include <boost/act/interlocked/assign/detail/assign_impl.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  detail::assign::are_valid_params< TargetType, SourceType >
, assign_result< TargetType, SourceType >
>
::type
assign( TargetType& destination, SourceType const& new_value )
{
  return detail::assign::assign( destination, new_value );
}

} } }

#endif
