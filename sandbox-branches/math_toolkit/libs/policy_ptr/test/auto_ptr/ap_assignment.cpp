// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/policy_ptr/smart_ptr.hpp>
#include <boost/policy_ptr/policy/auto_copy.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include "../detail/counter.hpp"

// Subsequent headers use this typedef
typedef boost::smart_ptr<boost::auto_copy> policy_ptr;

#include "../detail/hierarchy_virtual_dtor.hpp"
#include "../detail/source_sink.hpp"

using boost::unit_test_framework::test_suite;    

void assignment_test()
{
    {
        policy_ptr::to<base> ptr;
        ptr = base_source();
    }
    BOOST_CHECK(counter<base>::value() == 0);

    {
        policy_ptr::to<base> ptr;
        ptr = derived_source();
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<derived> ptr;
        ptr = derived_source();
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<base> ptr;
        ptr = policy_ptr::to<base>(new base);
    }
    BOOST_CHECK(counter<base>::value() == 0);

    {
        policy_ptr::to<base> ptr;
        ptr = policy_ptr::to<base>(new derived);
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<base> ptr;
        ptr = policy_ptr::to<derived>(new derived);
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    // Test moving from lvalues

    {
        policy_ptr::to<base> lhs;
        policy_ptr::to<base> rhs(new base);
        lhs = rhs;
    }
    BOOST_CHECK(counter<base>::value() == 0);

    {
        policy_ptr::to<base> lhs;
        policy_ptr::to<derived> rhs(new derived);
        lhs = rhs;
    }
    BOOST_CHECK(counter<derived>::value() == 0);

    {
        policy_ptr::to<derived> lhs;
        policy_ptr::to<derived> rhs(new derived);
        lhs = rhs;
    }
    BOOST_CHECK(counter<derived>::value() == 0);
}

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char* []) 
{
    boost::unit_test_framework::test_suite* test =
        BOOST_TEST_SUITE("move_copy assignment test");
    test->add(BOOST_TEST_CASE(&assignment_test));
    return test;
}
