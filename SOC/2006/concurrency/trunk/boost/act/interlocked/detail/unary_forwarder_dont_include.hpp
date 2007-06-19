/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_UNARY_FORWARDER_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/impl_meta.hpp>

#include <boost/act/interlocked/detail/forwarder.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO                            \
BOOST_ACT_INTERLOCKED_DETAIL_UNARY_FORWARDER_INFO

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_BEGIN()

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME
              , _result
              )
              < TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME( TargetType& target )
{
  typedef typename remove_volatile< TargetType >::type type;
  typedef typename BOOST_PP_CAT
                   ( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME
                   , _result
                   )
                   < TargetType >
                   ::type result_type;

  return detail::impl_meta
         <
           detail::BOOST_PP_CAT
                   ( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME
                   , _impl
                   )
         , TargetType
         >
         ::execute( target );
}

} } }

#define BOOST_ACT_INTERLOCKED_DETAIL_UNARY_FORWARDER_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_END()

#endif
