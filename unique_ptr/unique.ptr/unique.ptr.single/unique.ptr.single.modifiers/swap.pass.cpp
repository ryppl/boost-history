// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.modifiers]
// test swap

#include "unique_ptr.hpp"
#include <cassert>

struct A
{
    int state_;
    static int count;
    explicit A(int i) : state_(i) {++count;}
    A(const A& a) : state_(a.state_) {++count;}
    A& operator=(const A& a) {state_ = a.state_; return *this;}
    ~A() {--count;}

    friend bool operator==(const A& x, const A& y)
        {return x.state_ == y.state_;}
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

    Deleter() : state_(0) {}
    explicit Deleter(int i) : state_(i) {}

    int state() const {return state_;}

    void operator()(A* p) {delete p;}

    friend bool operator==(const Deleter& x, const Deleter& y)
        {return x.state_ == y.state_;}
};

void
swap(Deleter& x, Deleter& y)
{
    Deleter t(boost::move(x));
    x = boost::move(y);
    y = boost::move(t);
}

int main()
{
    {
    A* p1 = new A(1);
    boost::unique_ptr<A, Deleter> s1(p1, Deleter(1));
    A* p2 = new A(2);
    boost::unique_ptr<A, Deleter> s2(p2, Deleter(2));
    assert(s1.get() == p1);
    assert(*s1 == A(1));
    assert(s1.get_deleter().state() == 1);
    assert(s2.get() == p2);
    assert(*s2 == A(2));
    assert(s2.get_deleter().state() == 2);
    swap(s1, s2);
    assert(s1.get() == p2);
    assert(*s1 == A(2));
    assert(s1.get_deleter().state() == 2);
    assert(s2.get() == p1);
    assert(*s2 == A(1));
    assert(s2.get_deleter().state() == 1);
    }
}
