//
// Boost.Process
// Regression tests for the command_line class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/command_line.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static void
test_arguments_addition(void)
{
    bp::command_line cl("program");
    cl.argument("first").argument("second").argument("third");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_CHECK(args.size() == 3);
    BOOST_CHECK(args[0] == "first");
    BOOST_CHECK(args[1] == "second");
    BOOST_CHECK(args[2] == "third");
}

// ------------------------------------------------------------------------

static void
test_arguments_empty(void)
{
    bp::command_line cl("program");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_CHECK(args.size() == 0);
}

// ------------------------------------------------------------------------

static void
test_executable(void)
{
    bp::command_line cl("program");
    BOOST_CHECK(cl.get_executable() == "program");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("command_line test suite");

    test->add(BOOST_TEST_CASE(&test_executable));
    test->add(BOOST_TEST_CASE(&test_arguments_empty));
    test->add(BOOST_TEST_CASE(&test_arguments_addition));

    return test;
}
