/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_ASSIGN_IF_WAS_RELDEF_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_ASSIGN_IF_WAS_RELDEF_IMPL_HPP

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct assign_if_was_release_default_impl
{
  template< typename LeftType, typename RightType, typename OldType >
  static ResultType execute( LeftType& left, RightType& right
                           , OldType& old
                           )
  {
    return interlocked::assign_if_was( left, right, old );
  }
};


} } } }

#endif
