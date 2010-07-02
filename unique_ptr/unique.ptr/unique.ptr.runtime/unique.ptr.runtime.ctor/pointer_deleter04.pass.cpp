// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.ctor]
// Test unique_ptr(pointer, deleter) ctor

#include "unique_ptr.hpp"
#include <cassert>

// unique_ptr<T[], const D&>(pointer, d) does not requires CopyConstructible deleter

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

    Deleter(const Deleter&);
    Deleter& operator=(const Deleter&);
public:

    Deleter() : state_(5) {}

    int state() const {return state_;}
    void set_state(int s) {state_ = s;}

    void operator()(A* p) const {delete [] p;}
};

int main()
{
    {
    A* p = new A[3];
    assert(A::count == 3);
    Deleter d;
    boost::unique_ptr<A[], const Deleter&> s(p, d);
    assert(s.get() == p);
    assert(s.get_deleter().state() == 5);
    }
    assert(A::count == 0);
}