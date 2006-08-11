//
// Boost.Process
// Regression tests for the launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring>
#include <string>
#include <utility>

#include <boost/filesystem/operations.hpp>
#include <boost/process/child.hpp>
#include <boost/process/command_line.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
void
test_close_stdin(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("is-closed-stdin");

    bp::status s1 = bp::launcher().start(cl).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    bp::launcher l2;
    l2.set_stdin_behavior(bp::redirect_stream);
    bp::child c2 = l2.start(cl);
    c2.get_stdin() << "foo" << std::endl;
    c2.get_stdin().close();
    bp::status s2 = c2.wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

static
void
test_close_stdout(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("is-closed-stdout");

    bp::status s1 = bp::launcher().start(cl).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    bp::launcher l2;
    l2.set_stdout_behavior(bp::redirect_stream);
    bp::status s2 = l2.start(cl).wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

static
void
test_close_stderr(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("is-closed-stderr");

    bp::status s1 = bp::launcher().start(cl).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    bp::launcher l2;
    l2.set_stderr_behavior(bp::redirect_stream);
    bp::status s2 = l2.start(cl).wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

static
void
test_input(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("stdin-to-stdout");

    bp::launcher l;
    l.set_stdin_behavior(bp::redirect_stream);
    l.set_stdout_behavior(bp::redirect_stream);
    bp::child c = l.start(cl);

    bp::postream& os = c.get_stdin();
    bp::pistream& is = c.get_stdout();

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

static
void
test_output(bool out,
            const std::string& realmsg,
            const std::string& expmsg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument(out ? "echo-stdout" : "echo-stderr").argument(realmsg);

    bp::launcher l;
    if (out)
       l.set_stdout_behavior(bp::redirect_stream);
    else
       l.set_stderr_behavior(bp::redirect_stream);
    bp::child c = l.start(cl);

    std::string word;
    if (out) {
        bp::pistream& is = c.get_stdout();
        is >> word;
    } else {
        bp::pistream& is = c.get_stderr();
        is >> word;
    }
    BOOST_CHECK_EQUAL(word, expmsg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static void
test_stderr_fail(void)
{
    test_output(true, "message-stderr", "fail-stderr");
}

// ------------------------------------------------------------------------

static void
test_stderr_pass(void)
{
    test_output(true, "message-stderr", "message-stderr");
}

// ------------------------------------------------------------------------

static void
test_stdout_fail(void)
{
    test_output(false, "message-stdout", "fail-stdout");
}

// ------------------------------------------------------------------------

static void
test_stdout_pass(void)
{
    test_output(false, "message-stdout", "message-stdout");
}

// ------------------------------------------------------------------------

static void
test_merge_out_err(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("echo-stdout-stderr").argument("message-to-two-streams");

    bp::launcher l;
    l.set_stdout_behavior(bp::redirect_stream);
    l.set_merge_out_err(true);
    bp::child c = l.start(cl);

    bp::pistream& is = c.get_stdout();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "stdout");
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-two-streams");
    is >> word;
    BOOST_CHECK_EQUAL(word, "stderr");
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-two-streams");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static void
test_default_work_directory(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("pwd");

    bp::launcher l;
    l.set_stdout_behavior(bp::redirect_stream);
    BOOST_CHECK(bfs::equivalent(l.get_work_directory(),
                                bfs::current_path().string()));
    bp::child c = l.start(cl);

    bp::pistream& is = c.get_stdout();
    std::string dir;
    portable_getline(is, dir);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(l.get_work_directory()));
}

// ------------------------------------------------------------------------

static void
test_explicit_work_directory(void)
{
    bfs::path wdir = bfs::current_path() / "test.dir";

    bp::command_line cl(get_helpers_path());
    cl.argument("pwd");

    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        bp::launcher l;
        l.set_stdout_behavior(bp::redirect_stream);
        l.set_work_directory(wdir.string());
        BOOST_CHECK_EQUAL(l.get_work_directory(), wdir);
        bp::child c = l.start(cl);

        bp::pistream& is = c.get_stdout();
        std::string dir;
        portable_getline(is, dir);

        bp::status s = c.wait();
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        BOOST_REQUIRE(s.exited());
        BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

        BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(l.get_work_directory()));
    } catch(...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}

// ------------------------------------------------------------------------

static
std::pair< bool, std::string >
get_var_value(bp::launcher& l, const std::string& var)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_SET");

    l.set_stdout_behavior(bp::redirect_stream);
    bp::child c = l.start(cl);

    bp::pistream& is = c.get_stdout();
    std::string status;
    is >> status;
    std::string gotval;
    is >> gotval;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    return std::pair< bool, std::string >(status == "defined", gotval);
}

// ------------------------------------------------------------------------

static void
test_clear_environment(void)
{
#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("TO_BE_QUERIED", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_QUERIED") != NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable("TO_BE_QUERIED", "test") != 0);
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_QUERIED", buf, 5) != 0);
#endif

    bp::launcher l;
    l.clear_environment();

    std::pair< bool, std::string > p1 = get_var_value(l, "TO_BE_QUERIED");
    BOOST_REQUIRE(!p1.first);

    std::pair< bool, std::string > p2 = get_var_value(l, "PATH");
    BOOST_REQUIRE(!p2.first);

    std::pair< bool, std::string > p3 = get_var_value(l, "HOME");
    BOOST_REQUIRE(!p3.first);
}

// ------------------------------------------------------------------------

static void
test_unset_environment(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_UNSET");

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("TO_BE_UNSET", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_UNSET") != NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable("TO_BE_UNSET", "test") != 0);
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_UNSET", buf, 5) != 0);
#endif

    bp::launcher l;
    l.unset_environment("TO_BE_UNSET");
    std::pair< bool, std::string > p = get_var_value(l, "TO_BE_SET");
    BOOST_CHECK(!p.first);
}

// ------------------------------------------------------------------------

static void
test_set_environment(const std::string& value)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_SET");

#if defined(BOOST_PROCESS_POSIX_API)
    ::unsetenv("TO_BE_SET");
    BOOST_REQUIRE(::getenv("TO_BE_SET") == NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 0) == 0 ||
                  ::SetEnvironmentVariable("TO_BE_SET", NULL) != 0);
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 5) == 0);
#endif

    bp::launcher l;
    l.set_environment("TO_BE_SET", value);
    std::pair< bool, std::string > p = get_var_value(l, "TO_BE_SET");
    BOOST_CHECK(p.first);
    BOOST_CHECK_EQUAL(p.second, "'" + value + "'");
}

// ------------------------------------------------------------------------

static void
test_set_environment_empty(void)
{
    test_set_environment("");
}

// ------------------------------------------------------------------------

static void
test_set_environment_non_empty(void)
{
    test_set_environment("some-value");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("launcher test suite");

    test->add(BOOST_TEST_CASE(&test_close_stdin), 0, 10);
    test->add(BOOST_TEST_CASE(&test_close_stdout), 0, 10);
    test->add(BOOST_TEST_CASE(&test_close_stderr), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_merge_out_err), 0, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_clear_environment), 0, 10);
    test->add(BOOST_TEST_CASE(&test_unset_environment), 0, 10);
#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_set_environment_empty), 0, 10);
#endif
    test->add(BOOST_TEST_CASE(&test_set_environment_non_empty), 0, 10);

    return test;
}
