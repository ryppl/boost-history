// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime.ctor]
// Test unique_ptr<T[]>(pointer) ctor

#include "unique_ptr.hpp"

// unique_ptr<T[]>(pointer) ctor should require non-reference Deleter ctor
class Deleter
{
public:

    void operator()(void*) {}
};


int main()
{
    boost::unique_ptr<int[], Deleter&> p(new int);
}
