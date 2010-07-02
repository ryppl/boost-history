// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.asgn]
// Test unique_ptr move assignment

#include "unique_ptr.hpp"
#include <cassert>

// Can't copy from const lvalue

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
    const boost::unique_ptr<A> s(new A);
    boost::unique_ptr<A> s2;
    s2 = s;
    }
}