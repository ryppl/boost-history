//
// Boost.Process
// Regression tests for command line construction class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <string>

#include <boost/process/operations.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

// A predicate for BOOST_CHECK_EXCEPTION that always fails.
struct check_name
{
    std::string m_name;

    check_name(const std::string& name) :
        m_name(name)
    {
    }

    bool
    operator()(const bp::not_found_error< std::string >& e)
    {
        return e.get_value() == m_name;
    }
};

// ------------------------------------------------------------------------

static
void
test_find_default(void)
{
    std::string helpersname = bfs::path(get_helpers_path()).leaf();

    BOOST_CHECK_EXCEPTION(bp::find_executable_in_path(helpersname),
                          bp::not_found_error< std::string >,
                          check_name(helpersname));
}

// ------------------------------------------------------------------------

static
void
test_find_env(void)
{
    bfs::path orig = get_helpers_path();
    std::string helpersdir = orig.branch_path().string();
    std::string helpersname = orig.leaf();

#if defined(BOOST_PROCESS_POSIX_API)
    std::string oldpath = ::getenv("PATH");
    try {
        ::setenv("PATH", helpersdir.c_str(), 1);
        bfs::path found = bp::find_executable_in_path(helpersname);
        BOOST_CHECK(bfs::equivalent(orig, found));
        ::setenv("PATH", oldpath.c_str(), 1);
    } catch (...) {
        ::setenv("PATH", oldpath.c_str(), 1);
    }
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR oldpath[MAX_PATH];
    BOOST_REQUIRE(::GetEnvironmentVariable("PATH", oldpath, MAX_PATH) != 0);
    try {
        BOOST_REQUIRE(::SetEnvironmentVariable("PATH",
                                               TEXT(helpersdir.c_str())) != 0);
        bfs::path found = bp::find_executable_in_path(helpersname);
        BOOST_CHECK(bfs::equivalent(orig, found));
        BOOST_REQUIRE(::SetEnvironmentVariable("PATH", oldpath) != 0);
    } catch (...) {
        BOOST_REQUIRE(::SetEnvironmentVariable("PATH", oldpath) != 0);
    }
#endif
}

// ------------------------------------------------------------------------

static
void
test_find_param(void)
{
    bfs::path orig = get_helpers_path();
    std::string helpersdir = orig.branch_path().string();
    std::string helpersname = orig.leaf();

    bfs::path found = bp::find_executable_in_path(helpersname, helpersdir);
    BOOST_CHECK(bfs::equivalent(orig, found));
}

// ------------------------------------------------------------------------

static
void
test_progname(void)
{
    BOOST_CHECK_EQUAL(bp::executable_to_progname("foo"), "foo");
    BOOST_CHECK_EQUAL(bp::executable_to_progname("/foo"), "foo");
    BOOST_CHECK_EQUAL(bp::executable_to_progname("/foo/bar"), "bar");
    BOOST_CHECK_EQUAL(bp::executable_to_progname("///foo///bar"), "bar");
    BOOST_CHECK_EQUAL(bp::executable_to_progname("/foo/bar/baz"), "baz");

    if (bp_api_type == win32_api) {
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.exe"), "foo");
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.com"), "foo");
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.bat"), "foo");
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.bar"), "foo.bar");
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.bar.exe"), "foo.bar");
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.bar.com"), "foo.bar");
        BOOST_CHECK_EQUAL(bp::executable_to_progname("foo.bar.bat"), "foo.bar");
    }
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("executable test suite");

    test->add(BOOST_TEST_CASE(&test_find_default));
    test->add(BOOST_TEST_CASE(&test_find_env));
    test->add(BOOST_TEST_CASE(&test_find_param));
    test->add(BOOST_TEST_CASE(&test_progname));

    return test;
}
