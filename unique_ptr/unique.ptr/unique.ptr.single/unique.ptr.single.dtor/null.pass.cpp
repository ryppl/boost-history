// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.dtor]
// The deleter is not called if get() == 0

#include "unique_ptr.hpp"
#include <cassert>

class Deleter
{
    int state_;

    Deleter(Deleter&);
    Deleter& operator=(Deleter&);

public:
    Deleter() : state_(0) {}

    int state() const {return state_;}

    void operator()(void*) {++state_;}
};


int main()
{
    Deleter d;
    assert(d.state() == 0);
    {
    boost::unique_ptr<int, Deleter&> p(0, d);
    assert(p.get() == 0);
    assert(&p.get_deleter() == &d);
    }
    assert(d.state() == 0);
}
