// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.observers]
// test op->()

#include "unique_ptr.hpp"
#include <cassert>

struct A
{
    int i_;

    A() : i_(7) {}
};

int main()
{
    boost::unique_ptr<A[]> p(new A);
    assert(p->i_ == 7);
}
