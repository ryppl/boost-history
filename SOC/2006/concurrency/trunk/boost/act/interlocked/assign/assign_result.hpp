/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_RESULT_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_RESULT_HPP

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA

#include <boost/act/interlocked/detail/interlocked_result.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/mpl/if.hpp>
#include <boost/act/interlocked/assign/detail/assign_operation.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
struct assign_result
  : mpl::if_
    <
      detail::is_interlocked_bool< TargetType >
    , detail::binary_interlocked_result
      < detail::assign_operation
      , detail::old_value
      , bool
      >
    , detail::binary_interlocked_result
      < detail::assign_operation
      , detail::old_value
      , typename remove_volatile< TargetType >::type // ToDo: factor out
      >
    >
{
};

} } }

#else // Else: No assign support

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
struct assign_result
{
  BOOST_MPL_ASSERT_MSG
  (
    ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
  , INTERLOCKED_OPERATIONS_NOT_DEFINED_FOR_THIS_SYSTEM
  , ()
  );
};

} } }

#endif

#endif
