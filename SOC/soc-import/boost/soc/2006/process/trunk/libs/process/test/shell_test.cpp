//
// Boost.Process
// Regression tests for shell constructors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstdlib>
#include <string>

#include <boost/process/child.hpp>
#include <boost/process/launcher.hpp>
#include <boost/process/operations.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static void
test_shell_execution(void)
{
    std::string command;
    if (bp_api_type == posix_api)
        command = "echo test | sed 's,^,LINE-,'";
    else if (bp_api_type == win32_api)
        command = "if foo==foo echo LINE-test";
    else
        BOOST_REQUIRE(false);

    bp::launcher l;
    l.set_stdout_behavior(bp::redirect_stream);
    // XXX Without the following line, bash returns an exit status of 4,
    // which makes the test fail...  Why?  I don't know.
    l.set_merge_out_err(true);
    bp::child c = bp::launch_shell(l, command);

    bp::pistream& is = c.get_stdout();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "LINE-test");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("shell test suite");

    test->add(BOOST_TEST_CASE(&test_shell_execution));

    return test;
}
