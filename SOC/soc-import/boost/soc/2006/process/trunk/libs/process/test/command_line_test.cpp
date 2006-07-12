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

#include <cstdlib>

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
    BOOST_REQUIRE_EQUAL(args.size(),
        static_cast<bp::command_line::arguments_vector::size_type>(3));
    BOOST_CHECK_EQUAL(args[0], "first");
    BOOST_CHECK_EQUAL(args[1], "second");
    BOOST_CHECK_EQUAL(args[2], "third");
}

// ------------------------------------------------------------------------

static void
test_arguments_empty(void)
{
    bp::command_line cl("program");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_CHECK_EQUAL(args.size(),
        static_cast<bp::command_line::arguments_vector::size_type>(0));
}

// ------------------------------------------------------------------------

static void
test_arguments_types(void)
{
    bp::command_line cl("program");
    cl.argument("string").argument(123L);
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_REQUIRE_EQUAL(args.size(),
        static_cast<bp::command_line::arguments_vector::size_type>(2));
    BOOST_CHECK_EQUAL(args[0], "string");
    BOOST_CHECK_EQUAL(std::atol(args[1].c_str()), 123L);
}

// ------------------------------------------------------------------------

static void
test_executable(void)
{
    bp::command_line cl("program");
    BOOST_CHECK_EQUAL(cl.get_executable(), "program");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("command_line test suite");

    test->add(BOOST_TEST_CASE(&test_executable));
    test->add(BOOST_TEST_CASE(&test_arguments_empty));
    test->add(BOOST_TEST_CASE(&test_arguments_addition));
    test->add(BOOST_TEST_CASE(&test_arguments_types));

    return test;
}
