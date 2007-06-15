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

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_HEADER_RAW
#undef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_HEADER_RAW
#endif

#include BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_HEADER_RAW()

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_HEADER_RAW
#undef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_HEADER_RAW
#endif

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO
#undef BOOST_ACT_INTERLOCKED_DETAIL_CUSTOM_IMPL_INFO
#endif

#endif
