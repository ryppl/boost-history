// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.dltr.dflt]
//
// Test that default_delete's operator() requires a complete type

#include "unique_ptr.hpp"
#include <cassert>

struct A;

int main()
{
    boost::default_delete<A> d;
    A* p = 0;
    d(p);
}
