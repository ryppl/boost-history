/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

// Note: These tests could be a little bit better. Currently testing is done by
//       forcing compound CAS operations to use their default implementation
//       (which is to internally use assign_if_was).

#define BOOST_ACT_INTERLOCKED_FORCE_DEFAULTS

#include "../add_assign/add_assign_acquire.cpp"
