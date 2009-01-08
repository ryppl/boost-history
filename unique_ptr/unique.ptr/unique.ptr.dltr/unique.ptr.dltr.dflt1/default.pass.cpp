// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.dltr.dflt1]
//
// Test that default_delete<T[]> has a working default constructor

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
    boost::default_delete<A[]> d;
    A* p = new A[3];
    assert(A::count == 3);
    d(p);
    assert(A::count == 0);
}
