//
// Boost.Process
// Regression tests for the standard_launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring> // XXX For EXIT_* codes; should be hidden by the library.
#include <string>

#include <boost/process/child.hpp>
#include <boost/process/types.hpp>
#include <boost/process/standard_launcher.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

// XXX Extremely ugly way to determine helpers' path...
#if defined(__APPLE__)
#   define HELPERS_PATH "./bin/darwin/debug/helpers"
#else
#   define HELPERS_PATH "./bin/gcc/debug/helpers"
#endif

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

void
test_input(void)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("stdin-to-stdout");
    bp::attributes a(cl);

    bp::child c = bp::standard_launcher().start<bp::attributes>(a);

    bp::postream& os = c.get_input(bp::STDIN);
    bp::pistream& is = c.get_output(bp::STDOUT);

    os << "message-to-process" << std::endl;
    os.close();

    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

void
test_output(void)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("echo-two").argument(bp::STDOUT).argument(bp::STDERR).
        argument("message-to-process");
    bp::attributes a(cl);

    bp::child c = bp::standard_launcher().start<bp::attributes>(a);

    bp::pistream& is = c.get_output(bp::STDOUT);
    bp::desc_t dtmp;
    std::string word;
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, bp::STDOUT);
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, bp::STDERR);
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("standard_launcher test suite");

    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_output), 0, 10);

    return test;
}
