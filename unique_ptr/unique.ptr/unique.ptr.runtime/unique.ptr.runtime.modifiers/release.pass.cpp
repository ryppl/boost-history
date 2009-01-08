// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.modifiers]
// test release

#include "unique_ptr.hpp"
#include <cassert>

int main()
{
    boost::unique_ptr<int[]> p(new int[3]);
    int* i = p.get();
    int* j = p.release();
    assert(p.get() == 0);
    assert(i == j);
}
