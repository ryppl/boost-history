/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

/*

Note: If you are getting an error in this file, please post to the mailing list

*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_IS_READY_FOR_CLEANUP

// ToDo: Make this 80 characters max (push to macro invocation)
#if BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME, BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SEMANTICS )

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HEADER_RAW()                         \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT                                                                 \
  (                                                                            \
    BOOST_PP_CAT                                                               \
    ( <boost/act/interlocked/                                                  \
    , BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME                             \
    )                                                                          \
  , /detail/                                                                   \
  )                                                                            \
, BOOST_PP_CAT                                                                 \
  (                                                                            \
    BOOST_PP_CAT                                                               \
    (                                                                          \
      BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME                            \
      , _                                                                      \
      )                                                                        \
    , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NAME                            \
    )                                                                          \
  , .hpp>                                                                      \
  )                                                                            \
)

#else

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct BOOST_PP_CAT
       ( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
       , _default_impl
       );

template< typename ResultType, typename UnqualifiedType >
struct BOOST_PP_CAT
       ( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
       , _impl
       )
  : BOOST_PP_CAT
    ( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
    , _default_impl
    )
    < ResultType, UnqualifiedType > {};

} } } }

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HEADER_RAW()                         \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT                                                                 \
  (                                                                            \
    BOOST_PP_CAT                                                               \
    ( <boost/act/interlocked/                                                  \
    , BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME                             \
    )                                                                          \
  , /detail/                                                                   \
  )                                                                            \
, BOOST_PP_CAT                                                                 \
  (                                                                            \
    BOOST_PP_CAT                                                               \
    (                                                                          \
      BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME                            \
      , _                                                                      \
      )                                                                        \
    , default                                                                  \
    )                                                                          \
  , .hpp>                                                                      \
  )                                                                            \
)

#endif

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HEADER_RAW()

#endif
