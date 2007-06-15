/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONFIG_INTERLOCKED_HAS_INTERLOCKED_HPP
#define BOOST_ACT_CONFIG_INTERLOCKED_HAS_INTERLOCKED_HPP

#include <boost/act/interlocked/detail/cas_support_data.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA
#define BOOST_ACT_HAS_INTERLOCKED_OPERATIONS
#endif

#endif
