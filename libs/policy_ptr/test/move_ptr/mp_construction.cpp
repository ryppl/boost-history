// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/policy_ptr/smart_ptr.hpp>
#include <boost/policy_ptr/policy/move_copy.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include "../detail/counter.hpp"

// Subsequent headers use this typedef
typedef boost::smart_ptr<boost::move_copy> policy_ptr;

#include "../detail/hierarchy_virtual_dtor.hpp"
#include "../detail/source_sink.hpp"

using boost::unit_test_framework::test_suite;    

void construction_test()
{
    {
        policy_ptr::to<base> ptr(new base);
    }
    BOOST_CHECK(counter<base>::value() == 0);

    {
        policy_ptr::to<base> ptr(new derived);
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<derived> ptr(new derived);
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<base> ptr(policy_ptr::to<base>(new derived));
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<base> ptr(policy_ptr::to<derived>(new derived));
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        // The commented version fails to compile on EDG, but I don't
        // want this entire test to fail to compile. 

        //BOOST_MOVE_PTR<derived> ptr(BOOST_MOVE_PTR<derived>(new derived));
        policy_ptr::to<derived> ptr = policy_ptr::to<derived>(new derived);
    }
    BOOST_CHECK(counter<derived>::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("construction test");
    test->add(BOOST_TEST_CASE(&construction_test));
    return test;
}
