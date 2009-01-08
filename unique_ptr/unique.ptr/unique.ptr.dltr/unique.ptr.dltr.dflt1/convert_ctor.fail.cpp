// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.dltr.dflt1]
//
// Test that default_delete<T[]> does not have a working converting constructor

#include "unique_ptr.hpp"
#include <cassert>

struct A
{
};

struct B
    : public A
{
};

int main()
{
    boost::default_delete<B[]> d2;
    boost::default_delete<A[]> d1 = d2;
}
