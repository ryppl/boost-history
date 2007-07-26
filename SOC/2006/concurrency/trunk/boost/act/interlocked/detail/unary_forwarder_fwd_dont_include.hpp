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
#include <boost/type_traits/remove_cv.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/act/interlocked/semantics.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/forwarder_fwd.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_INFO                        \
BOOST_ACT_INTERLOCKED_DETAIL_UNARY_FORWARDER_INFO

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_BEGIN()

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_SEMANTICS >
  , detail::are_valid_store_style_params< TargetType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_SHORT_NAME( TargetType& target );

} } }

#define BOOST_ACT_INTERLOCKED_DETAIL_UNARY_FORWARDER_FWD_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_END()

#endif
