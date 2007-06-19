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

#include <boost/preprocessor/cat.hpp>

#if BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HAS_CUSTOM_IMPL

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

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HEADER_RAW()

#endif
