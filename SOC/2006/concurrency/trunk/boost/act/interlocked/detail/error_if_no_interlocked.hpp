/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_ERROR_IF_NO_INTERLOCKED_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_ERROR_IF_NO_INTERLOCKED_HPP

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>

#ifndef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS
#error Interlocked operations not supported in this implementation.
#endif

#endif
