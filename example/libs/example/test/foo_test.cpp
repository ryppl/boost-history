
// Copyright (C) 2008-2009 John Doe
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/example/example.hpp>
#include <boost/test/unit_test.hpp>

void foo_test()
{
    boost::example<int> a;
    a.foo();
}

boost::unit_test::test_suite* init_unit_test_suite(int, char* []) 
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE("foo test");
    test->add(BOOST_TEST_CASE(&foo_test));
    return test;
}
