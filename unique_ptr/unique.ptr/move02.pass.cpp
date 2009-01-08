// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr]
// Test namespace scope move functions

// move(rvalue int) should work

#include "unique_ptr.hpp"
#include <cassert>

int main()
{
    int j = boost::move(1);
    assert(j == 1);
}
