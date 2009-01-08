// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.observers]
// test op[](size_t)

#include "unique_ptr.hpp"
#include <cassert>

class A
{
    int state_;
    static int next_;
public:
    A() : state_(++next_) {}
    int get() const {return state_;}

    friend bool operator==(const A& x, int y)
        {return x.state_ == y;}

    A& operator=(int i) {state_ = i; return *this;}
};

int A::next_ = 0;

int main()
{
    boost::unique_ptr<A[]> p(new A[3]);
    assert(p[0] == 1);
    assert(p[1] == 2);
    assert(p[2] == 3);
    p[0] = 3;
    p[1] = 2;
    p[2] = 1;
    assert(p[0] == 3);
    assert(p[1] == 2);
    assert(p[2] == 1);
}
