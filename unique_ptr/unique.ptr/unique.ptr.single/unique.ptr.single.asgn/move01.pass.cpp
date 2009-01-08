// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.asgn]
// Test unique_ptr move assignment

#include "unique_ptr.hpp"
#include <cassert>

// test move assignment.  Should only require a MoveConstructible deleter, or if
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
    explicit Deleter(int i) : state_(i) {}

    int state() const {return state_;}

    void operator()(A* p) {delete p;}
};

int main()
{
    {
    boost::unique_ptr<A> s1(new A);
    A* p = s1.get();
    boost::unique_ptr<A> s2(new A);
    assert(A::count == 2);
    s2 = boost::move(s1);
    assert(A::count == 1);
    assert(s2.get() == p);
    assert(s1.get() == 0);
    }
    assert(A::count == 0);
    {
    boost::unique_ptr<A, Deleter> s1(new A);
    A* p = s1.get();
    boost::unique_ptr<A, Deleter> s2(new A);
    assert(A::count == 2);
    s2 = boost::move(s1);
    assert(s2.get() == p);
    assert(s1.get() == 0);
    assert(A::count == 1);
    assert(s2.get_deleter().state() == 5);
    assert(s1.get_deleter().state() == 0);
    }
    assert(A::count == 0);
    {
    Deleter d1(5);
    boost::unique_ptr<A, Deleter&> s1(new A, d1);
    A* p = s1.get();
    Deleter d2(6);
    boost::unique_ptr<A, Deleter&> s2(new A, d2);
    s2 = boost::move(s1);
    assert(s2.get() == p);
    assert(s1.get() == 0);
    assert(A::count == 1);
    assert(d1.state() == 0);
    assert(d2.state() == 5);
    }
    assert(A::count == 0);
}
