// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.ctor]
// Test unique_ptr move ctor

#include "unique_ptr.hpp"
#include <cassert>

// test move ctor.  Should only require a MoveConstructible deleter, or if
//    deleter is a reference, not even that.

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
    operator boost::detail::rv<Deleter>() {return boost::detail::rv<Deleter>(*this);}
    Deleter(boost::detail::rv<Deleter> r) : state_(r->state_) {r->state_ = 0;}
    Deleter& operator=(boost::detail::rv<Deleter> r)
    {
        state_ = r->state_;
        r->state_ = 0;
        return *this;
    }

    Deleter() : state_(5) {}

    int state() const {return state_;}

    void operator()(A* p) {delete p;}
};

class CDeleter
{
    int state_;

    CDeleter(CDeleter&);
    CDeleter& operator=(CDeleter&);
public:

    CDeleter() : state_(5) {}

    int state() const {return state_;}
    void set_state(int s) {state_ = s;}

    void operator()(A* p) {delete p;}
};

boost::unique_ptr<A>
source1()
{
    return boost::unique_ptr<A>(new A);
}

void sink1(boost::unique_ptr<A> p)
{
}

boost::unique_ptr<A, Deleter>
source2()
{
    return boost::unique_ptr<A, Deleter>(new A);
}

void sink2(boost::unique_ptr<A, Deleter> p)
{
}

boost::unique_ptr<A, CDeleter&>
source3()
{
    static CDeleter d;
    return boost::unique_ptr<A, CDeleter&>(new A, d);
}

void sink3(boost::unique_ptr<A, CDeleter&> p)
{
}

int main()
{
    sink1(source1());
    sink2(source2());
    sink3(source3());
    assert(A::count == 0);
}
