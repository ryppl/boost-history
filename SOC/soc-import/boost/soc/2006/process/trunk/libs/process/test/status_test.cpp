//
// Boost.Process
// Regression tests for the status class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring>
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/process/child.hpp>
#include <boost/process/command_line.hpp>
#include <boost/process/launcher.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
bp::child
launch_helper(const std::string& name)
{
    bp::command_line cl(get_helpers_path());
    cl.argument(name);
    return bp::launcher().start(cl);
}

// ------------------------------------------------------------------------

static
void
test_exit_failure(void)
{
    bp::status s = launch_helper("exit-failure").wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

static
void
test_exit_success(void)
{
    bp::status s = launch_helper("exit-success").wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("status test suite");

    test->add(BOOST_TEST_CASE(&test_exit_success), 0, 10);
    test->add(BOOST_TEST_CASE(&test_exit_failure), 0, 10);

    return test;
}
