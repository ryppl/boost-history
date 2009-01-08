// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.observers]
// test get_deleter()

#include "unique_ptr.hpp"
#include <cassert>

struct Deleter
{
    void operator()(void*) {}

    int test() {return 5;}
    int test() const {return 6;}
};

int main()
{
    {
    boost::unique_ptr<int[], Deleter> p;
    assert(p.get_deleter().test() == 5);
    }
    {
    const boost::unique_ptr<int[], Deleter> p;
    assert(p.get_deleter().test() == 6);
    }
}
