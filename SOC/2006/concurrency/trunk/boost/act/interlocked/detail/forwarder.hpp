/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO
#ifndef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_IS_READY_FOR_CLEANUP
#error Improper incusion of interlocked implementation header. Contact Boost.
#else
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_OP_RESULT_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_OP_HEADER_OPENER
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SEMANTICS
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO
#undef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_IS_READY_FOR_CLEANUP
#endif
#endif

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_HPP

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_BEGIN()                         \
<boost/act/interlocked/detail/forwarder_begin_dont_include.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_END()                           \
<boost/act/interlocked/detail/forwarder_end_dont_include.hpp>

#endif
