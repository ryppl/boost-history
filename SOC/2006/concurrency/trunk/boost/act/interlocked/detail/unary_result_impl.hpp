/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT

#include <boost/type_traits/remove_volatile.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER()                               \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( <boost/act/interlocked/                                        \
              , BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO                       \
              )                                                                \
, BOOST_PP_CAT( /detail/                                                       \
              , BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO         \
                            , _operation.hpp>                                  \
                            )                                                  \
              )                                                                \
)

#include BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER()

#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO, _result )
{
  typedef detail::unary_interlocked_result
          < detail::BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO
                                , _operation
                                )
          , detail::BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_ARITHMETIC_RETURN
          , typename remove_volatile< TargetType >::type
          >
          type;
};

} } }

#else

#include <boost/act/interlocked/detail/unsupported_result.hpp>

#endif

#undef BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO

#endif
