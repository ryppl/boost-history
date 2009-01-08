// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.ctor]
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

int main()
{
    {
    boost::unique_ptr<A> s(new A);
    A* p = s.get();
    boost::unique_ptr<A> s2 = boost::move(s);
    assert(s2.get() == p);
    assert(s.get() == 0);
    assert(A::count == 1);
    }
    assert(A::count == 0);
    {
    boost::unique_ptr<A, Deleter> s(new A);
    A* p = s.get();
    boost::unique_ptr<A, Deleter> s2 = boost::move(s);
    assert(s2.get() == p);
    assert(s.get() == 0);
    assert(A::count == 1);
    assert(s2.get_deleter().state() == 5);
    assert(s.get_deleter().state() == 0);
    }
    assert(A::count == 0);
    {
    CDeleter d;
    boost::unique_ptr<A, CDeleter&> s(new A, d);
    A* p = s.get();
    boost::unique_ptr<A, CDeleter&> s2 = boost::move(s);
    assert(s2.get() == p);
    assert(s.get() == 0);
    assert(A::count == 1);
    d.set_state(6);
    assert(s2.get_deleter().state() == d.state());
    assert(s.get_deleter().state() ==  d.state());
    }
    assert(A::count == 0);
}
