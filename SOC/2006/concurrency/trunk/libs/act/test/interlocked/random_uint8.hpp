/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_RANDOM_UINT8_HPP
#define BOOST_ACT_TEST_INTERLOCKED_RANDOM_UINT8_HPP

#include <boost/array.hpp>
#include <boost/act/interlocked/integer/types.hpp>

boost::array< boost::act::interlocked::uint_least8_t, 64 > const random_uint8
  = { 38,  71,  69,  12,  13
    , 90,  97,  52,  47,  14
    , 46,  21,  27,  18,  12
    , 49,  69,  22,  91,  101
    , 117, 29,  120, 105, 95
    , 14,  41,  25,  109, 114
    , 101, 100, 83,  56,  104
    , 113, 22,  122, 108, 127
    , 14,  85,  11,  97,  67
    , 63,  32,  57,  99,  106
    , 8,   49,  27,  82,  43
    , 56,  126, 127, 123, 37
    , 25,  23,  102, 82
    };

#endif
