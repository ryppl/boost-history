/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/act/interlocked/detail/full_name.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_HEADER_RAW
#undef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_HEADER_RAW
#endif

#define BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_HEADER_RAW()                  \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT                                                                 \
  (                                                                            \
    BOOST_PP_CAT                                                               \
    ( <boost/act/interlocked/                                                  \
    , BOOST_PP_TUPLE_ELEM( 2, 0                                                \
                         , BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO       \
                         )                                                     \
    )                                                                          \
  , /detail/                                                                   \
  )                                                                            \
, BOOST_PP_CAT                                                                 \
  (                                                                            \
    BOOST_PP_CAT                                                               \
    (                                                                          \
      BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME                                 \
        ( BOOST_PP_TUPLE_ELEM( 2, 0                                            \
                             , BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO   \
                             )                                                 \
        , BOOST_PP_TUPLE_ELEM( 2, 1                                            \
                             , BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO   \
                             )                                                 \
        )                                                                      \
      , _                                                                      \
      )                                                                        \
    , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NAME                            \
    )                                                                          \
  , .hpp>                                                                      \
  )                                                                            \
)

#include BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_HEADER_RAW()

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_HEADER_RAW
#undef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_HEADER_RAW
#endif

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO
#undef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO
#endif

#endif
