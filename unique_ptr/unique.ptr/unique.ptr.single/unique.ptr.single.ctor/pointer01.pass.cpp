// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.ctor]
// Test unique_ptr(pointer) ctor

#include "unique_ptr.hpp"
#include <cassert>

// unique_ptr(pointer) ctor should only require default Deleter ctor

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

    Deleter(Deleter&);
    Deleter& operator=(Deleter&);

public:
    Deleter() : state_(5) {}

    int state() const {return state_;}

    void operator()(A* p) {delete p;}
};

int main()
{
    {
    A* p = new A;
    assert(A::count == 1);
    boost::unique_ptr<A> s(p);
    assert(s.get() == p);
    }
    assert(A::count == 0);
    {
    A* p = new A;
    assert(A::count == 1);
    boost::unique_ptr<A, Deleter> s(p);
    assert(s.get() == p);
    assert(s.get_deleter().state() == 5);
    }
    assert(A::count == 0);
}
