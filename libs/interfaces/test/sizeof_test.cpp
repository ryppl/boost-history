// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <cstddef>
#include <string>
#include <boost/interfaces.hpp>
#include <boost/static_assert.hpp>
#include "./detail/hierarchies.hpp"
#include "./detail/template_hierarchies.hpp"

struct test {
    void* x;
    void* y;
};

int main()
{
    using namespace boost::interfaces::test;
    BOOST_STATIC_ASSERT(sizeof(ifirst) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(iderived1) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(iderived2) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(iderived4) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(itemplate_first<int, const char*>) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(itemplate_derived1<int, const char*>) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(itemplate_derived2<int, const char*>) == sizeof(test));
    BOOST_STATIC_ASSERT(sizeof(itemplate_derived4<int, const char*>) == sizeof(test));
}
