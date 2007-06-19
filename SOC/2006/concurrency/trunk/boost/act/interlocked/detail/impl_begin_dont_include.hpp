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

#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME                           \
BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SEMANTICS                            \
BOOST_PP_TUPLE_ELEM( 2, 1, BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO )

#include <boost/act/interlocked/detail/full_name.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME                            \
BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME                                         \
( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME                                 \
, BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SEMANTICS                                  \
)

#include <boost/act/interlocked/detail/cas_support.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HAS_CUSTOM_IMPL                      \
BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL                                   \
( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME                                 \
, BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SEMANTICS                                  \
)

namespace boost { namespace act { namespace interlocked { namespace detail {

#if BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HAS_CUSTOM_IMPL

struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME, _impl );

#else

struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
                   , _default_impl
                   );

typedef BOOST_PP_CAT
        ( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
        , _default_impl
        )
        BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME, _impl );

#endif

} } } }

#endif
