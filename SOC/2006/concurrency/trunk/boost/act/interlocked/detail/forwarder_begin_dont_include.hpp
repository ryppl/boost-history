/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

/*

Note: If you are getting an error in this file, please post to the mailing list

*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/detail/full_name.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME                      \
BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SEMANTICS                       \
BOOST_PP_TUPLE_ELEM( 2, 1, BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME                       \
BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME                                         \
( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME                            \
, BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SEMANTICS                             \
)

#include <boost/act/interlocked/detail/impl.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO                                 \
BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()

#endif
