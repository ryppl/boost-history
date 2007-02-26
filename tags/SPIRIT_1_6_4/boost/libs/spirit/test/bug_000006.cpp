/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// comment_p and end of data bug

#include <boost/limits.hpp>
#include <boost/test/included/unit_test_framework.hpp>
#include <boost/spirit/utility/confix.hpp>

using namespace boost;
using namespace unit_test_framework;
using namespace boost::spirit;

///////////////////////////////////////////////////////////////////////////////
//  test function
void bug_000006_test()
{
    BOOST_CHECK(parse("#some comment", comment_p('#')).full);
}

///////////////////////////////////////////////////////////////////////////////
//  Boost.Test entry point
test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE("bug_000006 test");

    test->add(BOOST_TEST_CASE(&bug_000006_test));

    return test;
}

