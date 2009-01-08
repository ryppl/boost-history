// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.ctor]
// Test unique_ptr converting move ctor

#include "unique_ptr.hpp"
#include <cassert>

// test converting move ctor.  Should only require a MoveConstructible deleter, or if
//    deleter is a reference, not even that.
// Explicit version

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

template <class T>
class CDeleter
{
    int state_;

    CDeleter(CDeleter&);
    CDeleter& operator=(CDeleter&);
public:

    CDeleter() : state_(5) {}

    int state() const {return state_;}
    void set_state(int s) {state_ = s;}

    void operator()(T* p) {delete p;}
};

int main()
{
    {
    CDeleter<A> d;
    boost::unique_ptr<B, CDeleter<A>&> s(new B, d);
    A* p = s.get();
    boost::unique_ptr<A, CDeleter<A>&> s2(boost::move(s));
    assert(s2.get() == p);
    assert(s.get() == 0);
    assert(A::count == 1);
    assert(B::count == 1);
    d.set_state(6);
    assert(s2.get_deleter().state() == d.state());
    assert(s.get_deleter().state() ==  d.state());
    }
    assert(A::count == 0);
    assert(B::count == 0);
}
