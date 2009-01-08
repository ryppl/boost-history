// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.single.ctor]
// Test unique_ptr converting move ctor

#include "unique_ptr.hpp"
#include <cassert>

// Do not convert from an array unique_ptr

struct A
{
};

struct Deleter
{
    void operator()(void*) {}
};

int main()
{
    boost::unique_ptr<A[], Deleter> s;
    boost::unique_ptr<A, Deleter> s2(boost::move(s));
}
