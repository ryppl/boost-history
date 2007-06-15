/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_SUB_ASSIGN_DETAIL_SUB_ASSIGN_RELEASE_DEFAULT_HPP
#define BOOST_ACT_INTERLOCKED_SUB_ASSIGN_DETAIL_SUB_ASSIGN_RELEASE_DEFAULT_HPP

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO
#undef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO
#endif

#define BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO                         \
( subtract_assign, release, - )

#include <boost/act/interlocked/detail/binary_default_impl.hpp>

#endif
