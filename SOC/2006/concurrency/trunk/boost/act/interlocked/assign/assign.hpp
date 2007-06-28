/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( assign, acq_rel )

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/act/interlocked/assign/assign_acq_rel.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  detail::are_valid_store_style_params< TargetType, SourceType const >
, remove_cv< TargetType >
>
::type
assign( TargetType& destination, SourceType const& new_value )
{
  return interlocked::assign_acq_rel( destination, new_value );

}

} } }

#endif

#endif
