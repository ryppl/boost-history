/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_RETRIEVE_DETAIL_RETRIEVE_DEFAULT_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_RETRIEVE_DETAIL_RETRIEVE_DEFAULT_IMPL_HPP

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA

#if BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_RETRIEVE // If: volatile impl

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct retrieve_default_impl
{
  template< typename TargetType >
  static ResultType execute( TargetType volatile& target )
  {
    return target;
  }
};

} } } }

#else // Else: Custom interlocked retrieve for this system (no default)

#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct retrieve_default_impl
{
  template< typename TargetType >
  static ResultType execute( TargetType& target )
  {
    BOOST_MPL_ASSERT_MSG
    (
      ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
    , NO_DEFAULT_INTERLOCKED_RETRIEVE_IMPLEMENTATION_FOR_THIS_SYSTEM
    , ()
    );
  }
};

} } } }

#endif

#else // Else: No interlocked operations for this system

#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct retrieve_default_impl
{
  template< typename TargetType >
  static ResultType execute( TargetType& target )
  {
    BOOST_MPL_ASSERT_MSG
    (
      ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
    , INTERLOCKED_OPERATIONS_NOT_DEFINED_FOR_THIS_SYSTEM
    , ()
    );
  }
};


} } } }

#endif

#endif
