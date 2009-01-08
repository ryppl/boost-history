// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.observers]
// test op*()

#include "unique_ptr.hpp"
#include <cassert>

int main()
{
    {
    boost::unique_ptr<int> p(new int(3));
    if (p)
        ;
    else
        assert(false);
    if (!p)
        assert(false);
    }
    {
    boost::unique_ptr<int> p;
    if (!p)
        ;
    else
        assert(false);
    if (p)
        assert(false);
    }
}
