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

void source_sink_test()
{
    base_sink(base_source());
    BOOST_CHECK(counter<base>::value() == 0);

    base_sink(derived_source());
    BOOST_CHECK(counter<derived>::value() == 0);

    derived_sink(derived_source());
    BOOST_CHECK(counter<derived>::value() == 0);

    base_sink(policy_ptr::to<base>(new base));
    BOOST_CHECK(counter<derived>::value() == 0);

    base_sink(policy_ptr::to<base>(new derived));
    BOOST_CHECK(counter<derived>::value() == 0);

    base_sink(policy_ptr::to<derived>(new derived));
    BOOST_CHECK(counter<derived>::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("source sink test");
    test->add(BOOST_TEST_CASE(&source_sink_test));
    return test;
}
