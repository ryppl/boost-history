// Copyright 2009 Howard Hinnant.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation

// [unique.ptr.runtime]
// Test unique_ptr<T[]>::pointer type

#include "unique_ptr.hpp"
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

struct Deleter
{
    struct pointer {};
};

int main()
{
    {
    typedef boost::unique_ptr<int[]> P;
    using namespace boost;
    BOOST_STATIC_ASSERT((is_same<P::pointer, int*>::value));
    }
    {
    typedef boost::unique_ptr<int[], Deleter> P;
    using namespace boost;
    BOOST_STATIC_ASSERT((is_same<P::pointer, Deleter::pointer>::value));
    }
}
