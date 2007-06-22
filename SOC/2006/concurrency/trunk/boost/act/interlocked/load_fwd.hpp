/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_RETRIEVE_FWD_HPP
#define BOOST_ACT_INTERLOCKED_RETRIEVE_FWD_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  detail::are_valid_assign_style_params< TargetType >
, remove_volatile< TargetType >
>
::type
retrieve( TargetType const& target );

} } }

#endif
