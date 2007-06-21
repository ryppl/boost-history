/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

// ToDo: Do fancy dance to undef old macros

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO
#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_IS_READY_FOR_CLEANUP
#error Improper incusion of interlocked implementation header. Contact Boost.
#else
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_LOCAL_INCLUDE
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HAS_CUSTOM_IMPL
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SHORT_NAME
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_SEMANTICS
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO
#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_IS_READY_FOR_CLEANUP
#endif
#endif

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_HPP

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()                              \
<boost/act/interlocked/detail/impl_begin_dont_include.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_END()                                \
<boost/act/interlocked/detail/impl_end_dont_include.hpp>

#endif
