// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.ctor]
// Test unique_ptr default ctor

#include "unique_ptr.hpp"
#include <cassert>

// default unique_ptr ctor should only require default Deleter ctor
class Deleter
{
    int state_;

    Deleter(Deleter&);
    Deleter& operator=(Deleter&);

public:
    Deleter() : state_(5) {}

    int state() const {return state_;}

    void operator()(void*) {}
};


int main()
{
    {
    boost::unique_ptr<int> p;
    assert(p.get() == 0);
    }
    {
    boost::unique_ptr<int, Deleter> p;
    assert(p.get() == 0);
    assert(p.get_deleter().state() == 5);
    }
}
