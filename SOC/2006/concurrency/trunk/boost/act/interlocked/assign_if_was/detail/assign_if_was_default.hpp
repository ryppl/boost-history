/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_IF_WAS_DEFAULT_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_IF_WAS_DEFAULT_IMPL_HPP

#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct assign_if_was_default_impl
{
  template< typename LeftType, typename RightType, typename ConditionType >
  static ResultType execute( LeftType&, RightType&, ConditionType& )
  {
    BOOST_MPL_ASSERT_MSG
    (
      ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
    , NO_DEFAULT_INTERLOCKED_ASSIGN_IF_WAS_IMPLEMENTATION
    , ()
    );
  }
};

} } } }

#endif
