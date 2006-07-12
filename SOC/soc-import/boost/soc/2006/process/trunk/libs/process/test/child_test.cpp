//
// Boost.Process
// Regression tests for the child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring> // XXX For EXIT_* codes; should be hidden by the library.
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/process/child.hpp>
#include <boost/process/types.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

// XXX Extremely ugly way to determine helpers' path...
#if defined(__APPLE__)
#   define HELPERS_PATH \
        (bfs::initial_path() / "./bin/darwin/debug/helpers").string()
#else
#   define HELPERS_PATH \
        (bfs::initial_path() / "./bin/gcc/debug/helpers").string()
#endif

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

void
test_input(void)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("stdin-to-stdout");
    bp::attributes a(cl);

    bp::launcher l;
    l.input(bp::STDIN).output(bp::STDOUT);
    bp::child c = l.start<bp::attributes>(a);

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
test_output(bp::desc_t desc,
            const std::string& outname,
            const std::string& realmsg,
            const std::string& expmsg)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("echo-" + outname).argument(realmsg);
    bp::attributes a(cl);

    bp::launcher l;
    l.output(desc);
    bp::child c = l.start<bp::attributes>(a);

    bp::pistream& is = c.get_output(desc);
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, expmsg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static void
test_stderr_fail(void)
{
    test_output(bp::STDERR, "stderr", "message-stderr", "fail-stderr");
}

// ------------------------------------------------------------------------

static void
test_stderr_pass(void)
{
    test_output(bp::STDERR, "stderr", "message-stderr", "message-stderr");
}

// ------------------------------------------------------------------------

static void
test_stdout_fail(void)
{
    test_output(bp::STDOUT, "stdout", "message-stdout", "fail-stdout");
}

// ------------------------------------------------------------------------

static void
test_stdout_pass(void)
{
    test_output(bp::STDOUT, "stdout", "message-stdout", "message-stdout");
}

// ------------------------------------------------------------------------

static void
test_default_work_directory(void)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("pwd");
    bp::attributes a(cl);

    bp::launcher l;
    l.output(bp::STDOUT);
    bp::child c = l.start<bp::attributes>(a);

    bp::pistream& is = c.get_output(bp::STDOUT);
    std::string dir;
    is >> dir;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(dir, a.get_work_directory());
}

// ------------------------------------------------------------------------

static void
test_explicit_work_directory(void)
{
    bfs::path wdir = bfs::current_path() / "test.dir";

    bp::command_line cl(HELPERS_PATH);
    cl.argument("pwd");
    bp::attributes a(cl, wdir.string());

    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        bp::launcher l;
        l.output(bp::STDOUT);
        bp::child c = l.start<bp::attributes>(a);

        bp::pistream& is = c.get_output(bp::STDOUT);
        std::string dir;
        is >> dir;

        bp::status s = c.wait();
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        BOOST_REQUIRE(s.exited());
        BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

        BOOST_CHECK_EQUAL(dir, a.get_work_directory());
    } catch(...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("child test suite");

    bfs::initial_path();

    test->add(BOOST_TEST_CASE(&test_stdout_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory), 0, 10);

    return test;
}
