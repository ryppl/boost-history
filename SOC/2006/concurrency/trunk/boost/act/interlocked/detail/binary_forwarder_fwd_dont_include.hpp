/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_IS_ADDITIVE          \
BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_AFFIX_         \
            , BOOST_PP_TUPLE_ELEM                                              \
              ( 3, 2                                                           \
              , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_INFO         \
              )                                                                \
            )

#include <boost/act/interlocked/detail/forwarder_fwd.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_INFO                        \
( BOOST_PP_TUPLE_ELEM( 3, 0                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_INFO  \
                     )                                                         \
, BOOST_PP_TUPLE_ELEM( 3, 1                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_INFO  \
                     )                                                         \
)

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_BEGIN()

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename OperandType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::
#if BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_IS_ADDITIVE
    are_valid_additive_params
#else
    are_valid_arithmetic_params
#endif
    < TargetType, OperandType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_FULL_NAME( TargetType& target
                                                    , OperandType const& operand
                                                    );

} } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_FWD_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FWD_END()

#endif
