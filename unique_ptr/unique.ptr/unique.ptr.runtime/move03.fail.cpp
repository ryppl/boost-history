// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.asgn]
// Test unique_ptr move assignment

#include "unique_ptr.hpp"
#include <cassert>

// Can't copy from lvalue

struct A
{
    static int count;
    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

class Deleter
{
    int state_;

public:

    Deleter() : state_(5) {}

    int state() const {return state_;}

    void operator()(A* p) {delete p;}
};

int main()
{
    {
    boost::unique_ptr<A, Deleter> s(new A);
    A* p = s.get();
    boost::unique_ptr<A, Deleter> s2;
    s2 = s;
    assert(s2.get() == p);
    assert(s.get() == 0);
    assert(A::count == 1);
    }
    assert(A::count == 0);
}
