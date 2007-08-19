/* Boost test_div.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define TEST_MODDIV_SMALL test_div_small
#define TEST_MODDIV_MEDIUM test_div_medium
#define MODDIV_OP /

#include "impl_moddiv.inl"

#undef MODDIV_OP
#undef TEST_MODDIV_MEDIUM
#undef TEST_MODDIV_SMALL
