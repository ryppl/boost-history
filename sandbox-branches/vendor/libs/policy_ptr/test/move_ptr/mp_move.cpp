// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// This test should compile and execute successfully for both static_move_ptr
// and dynamic_move_ptr.

#include <boost/policy_ptr/smart_ptr.hpp>
#include <boost/policy_ptr/policy/move_copy.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include "../detail/counter.hpp"

// Subsequent headers use this typedef
typedef boost::smart_ptr<boost::move_copy> policy_ptr;

#include "../detail/hierarchy_virtual_dtor.hpp"
#include "../detail/source_sink.hpp"

#include <iostream>

using boost::unit_test_framework::test_suite;    

void move_test()
{
    {
        policy_ptr::to<base> ptr(new base);
        base_sink(move(ptr));
        std::cout << counter<base>::value()<< "\n";
        BOOST_CHECK(counter<base>::value() == 0);
    }

    {
        policy_ptr::to<base> ptr(new derived);
        base_sink(move(ptr));
        std::cout << counter<derived>::value()<< "\n";
        BOOST_CHECK(counter<derived>::value() == 0);
    }

    {
        policy_ptr::to<derived> ptr(new derived);
        base_sink(move(ptr));
        std::cout << counter<derived>::value()<< "\n";
        BOOST_CHECK(counter<derived>::value() == 0);
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("move test");
    test->add(BOOST_TEST_CASE(&move_test));
    return test;
}
