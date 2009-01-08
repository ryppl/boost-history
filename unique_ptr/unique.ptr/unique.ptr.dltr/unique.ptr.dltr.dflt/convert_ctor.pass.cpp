// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.dltr.dflt]
//
// Test that default_delete has a working converting constructor

#include "unique_ptr.hpp"
#include <cassert>

struct A
{
    static int count;
    A() {++count;}
    A(const A&) {++count;}
    virtual ~A() {--count;}
};

int A::count = 0;

struct B
    : public A
{
    static int count;
    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};

int B::count = 0;

int main()
{
    boost::default_delete<B> d2;
    boost::default_delete<A> d1 = d2;
    A* p = new B;
    assert(A::count == 1);
    assert(B::count == 1);
    d1(p);
    assert(A::count == 0);
    assert(B::count == 0);
}
