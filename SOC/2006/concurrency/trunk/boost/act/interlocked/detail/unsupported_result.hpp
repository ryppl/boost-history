/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME, _result )
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
