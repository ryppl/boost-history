/* Boost test_sub.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define TEST_AORS_SMALL test_sub_small
#define TEST_AORS_LARGE test_sub_large
#define AORS_OP -=

#include "impl_aors.inl"

#undef AORS_OP
#undef TEST_AORS_LARGE
#undef TEST_AORS_SMALL
