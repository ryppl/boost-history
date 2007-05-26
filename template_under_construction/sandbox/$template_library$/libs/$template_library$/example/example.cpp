// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

$template_start$
$template_cpp_copyright$

//[ $template_library$_example
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

#include <boost/$template_library$.hpp>

void $template_library$_example()
{
    boost::$template_library$::$template_library$<int> object;
    int x = 0;

    BOOST_CHECK_EQUAL(object.get_nothing_count(),0);
    object.nothing(x);
    BOOST_CHECK_EQUAL(object.get_nothing_count(),1);

    BOOST_CHECK_EQUAL(object.get_call_count(),1);
    object.nothing(x,4);
    BOOST_CHECK_EQUAL(object.get_call_count(),2);
    BOOST_CHECK_EQUAL(object.get_nothing_count(),5);

} // end void $template_library$_example

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE( "RPC test suite" );
    test->add(BOOST_TEST_CASE(&$template_library$_example));
    return test;
}

//]