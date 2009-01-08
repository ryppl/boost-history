// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.ctor]
// Test default unique_ptr<T[]> ctor

#include "unique_ptr.hpp"
#include <cassert>

// default unique_ptr<T[]> ctor shouldn't require complete type

struct A;

class Deleter
{
    int state_;

    Deleter(Deleter&);
    Deleter& operator=(Deleter&);

public:
    Deleter() : state_(5) {}

    int state() const {return state_;}

    void operator()(A* p);
};

void check(int i);

template <class D = boost::default_delete<A> >
struct B
{
    boost::unique_ptr<A[], D> a_;
    B() {}
    ~B();

    A* get() const {return a_.get();}
    D& get_deleter() {return a_.get_deleter();}
};

int main()
{
    {
    B<> s;
    assert(s.get() == 0);
    }
    check(0);
    {
    B<Deleter> s;
    assert(s.get() == 0);
    assert(s.get_deleter().state() == 5);
    }
    check(0);
}

struct A
{
    static int count;
    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

void Deleter::operator()(A* p) {delete p;}

void check(int i)
{
    assert(A::count == i);
}

template <class D>
B<D>::~B() {}
