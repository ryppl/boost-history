/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_DEFAULT_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_DEFAULT_IMPL_HPP

#include <boost/act/interlocked/assign/assign.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct assign_acquire_default_impl
{
  template< typename LeftType, typename RightType >
  static ResultType execute( LeftType& left, RightType& right )
  {
    return interlocked::assign( left, right );
  }
};


} } } }

#endif
