// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.asgn]
// Test unique_ptr converting move assignment

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

template <class T>
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

    template <class U>
        Deleter(Deleter<U> d,
            typename boost::enable_if_c
            <
                boost::detail::is_convertible<U, T>::value
            >::type* = 0)
            : state_(d.state()) {}

    template <class U>
        Deleter& operator=(Deleter<U> d)
        {
            state_ = d.state();
            return *this;
        }

    int state() const {return state_;}

    void operator()(T* p) {delete p;}
};

int main()
{
    {
    boost::unique_ptr<B, Deleter<B> > s(new B);
    A* p = s.get();
    boost::unique_ptr<A, Deleter<A> > s2(new A);
    assert(A::count == 2);
    s2 = (boost::move(s));
    assert(s2.get() == p);
    assert(s.get() == 0);
    assert(A::count == 1);
    assert(B::count == 1);
    assert(s2.get_deleter().state() == 5);
    assert(s.get_deleter().state() == 0);
    }
    assert(A::count == 0);
    assert(B::count == 0);
}
