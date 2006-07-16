//
// Boost.Process
// Regression tests for the attributes class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/process/attributes.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static void
test_command_line(void)
{
    bp::command_line cl("program");
    bp::attributes a(cl);
    const bp::command_line& rcl = a.get_command_line();
    BOOST_CHECK_EQUAL(rcl.get_executable(), "program");
}

// ------------------------------------------------------------------------

static void
test_default_work_directory(void)
{
    bp::command_line cl("program");
    bp::attributes a(cl);
    BOOST_CHECK(bfs::equivalent(a.get_work_directory(),
                                bfs::current_path().string()));
}

// ------------------------------------------------------------------------

static void
test_explicit_work_directory(void)
{
    bp::command_line cl("program");
    bp::attributes a(cl, "/work/directory");
    BOOST_CHECK_EQUAL(a.get_work_directory(), "/work/directory");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("attributes test suite");

    test->add(BOOST_TEST_CASE(&test_command_line));
    test->add(BOOST_TEST_CASE(&test_default_work_directory));
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory));

    return test;
}
