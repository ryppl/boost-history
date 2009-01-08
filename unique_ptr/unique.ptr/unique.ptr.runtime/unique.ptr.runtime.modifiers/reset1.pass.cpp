// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.modifiers]
// test reset

#include "unique_ptr.hpp"
#include <cassert>

struct A
{
    static int count;
    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int main()
{
    {
    boost::unique_ptr<A[]> p(new A[3]);
    assert(A::count == 3);
    A* i = p.get();
    p.reset();
    assert(A::count == 0);
    assert(p.get() == 0);
    }
    assert(A::count == 0);
    {
    boost::unique_ptr<A[]> p(new A[4]);
    assert(A::count == 4);
    A* i = p.get();
    p.reset(new A[5]);
    assert(A::count == 5);
    }
    assert(A::count == 0);
}
