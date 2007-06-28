/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_XOR_ASSIGN_DETAIL_XOR_ASSIGN_DEFAULT_HPP
#define BOOST_ACT_INTERLOCKED_XOR_ASSIGN_DETAIL_XOR_ASSIGN_DEFAULT_HPP

#include <boost/act/interlocked/detail/binary_default.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_INFO                       \
( xor_assign, acq_rel, ^ )

#include BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT()

#endif
