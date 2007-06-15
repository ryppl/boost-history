/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO
#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_READY_FOR_CLEANUP
#error Improper incusion of interlocked implementation header. Contact Boost.
#else
#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_ADDITIVE
#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO
#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_READY_FOR_CLEANUP
#endif
#endif

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_HPP

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_AFFIX_not_additive 0
#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_AFFIX_additive     1

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER()                        \
<boost/act/interlocked/detail/binary_forwarder_dont_include.hpp>

#endif
