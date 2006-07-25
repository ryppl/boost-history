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

#include <cstring> // XXX For EXIT_* codes; should be hidden by the library.
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/process/child.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

void
test_flags(void)
{
    bp::launcher l1(bp::launcher::REDIR_NONE);
    BOOST_CHECK_EQUAL(l1.get_flags(), (int)bp::launcher::REDIR_NONE);

    bp::launcher l2(bp::launcher::REDIR_STDIN);
    BOOST_CHECK_EQUAL(l2.get_flags(), (int)bp::launcher::REDIR_STDIN);

    bp::launcher l3;
    BOOST_CHECK_EQUAL(l3.get_flags(), (int)bp::launcher::REDIR_ALL);

    bp::launcher l4;
    l4.set_flags(bp::launcher::REDIR_NONE);
    BOOST_CHECK_EQUAL(l4.get_flags(), (int)bp::launcher::REDIR_NONE);
    l4.set_flags(bp::launcher::REDIR_STDOUT);
    BOOST_CHECK_EQUAL(l4.get_flags(), (int)bp::launcher::REDIR_STDOUT);
}

// ------------------------------------------------------------------------

void
test_input(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("stdin-to-stdout");
    bp::attributes a;

    bp::launcher l(bp::launcher::REDIR_STDIN | bp::launcher::REDIR_STDOUT);
    bp::child c = l.start(cl, a);

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

void
test_output(bool out,
            const std::string& realmsg,
            const std::string& expmsg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument(out ? "echo-stdout" : "echo-stderr").argument(realmsg);
    bp::attributes a;

    // XXX WTH the following two lines result in REDIR_* being undefined
    // symbols?!
    // bp::launcher l(out ? bp::launcher::REDIR_STDOUT :
    //                      bp::launcher::REDIR_STDERR);
    int flags;
    if (out)
        flags = bp::launcher::REDIR_STDOUT;
    else
        flags = bp::launcher::REDIR_STDERR;
    bp::launcher l(flags);
    bp::child c = l.start(cl, a);

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

void
test_merge(const std::string& msg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("echo-stdout-stderr").argument(msg);
    bp::attributes a;

    bp::launcher l(bp::launcher::REDIR_STDOUT |
                   bp::launcher::REDIR_STDERR_TO_STDOUT);
    bp::child c = l.start(cl, a);

    bp::pistream& is = c.get_stdout();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "stdout");
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);
    is >> word;
    BOOST_CHECK_EQUAL(word, "stderr");
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static void
test_merge_out_err(void)
{
    test_merge("message-to-two-streams");
}

// ------------------------------------------------------------------------

static void
test_default_work_directory(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("pwd");
    bp::attributes a;

    bp::launcher l(bp::launcher::REDIR_STDOUT);
    bp::child c = l.start(cl, a);

    bp::pistream& is = c.get_stdout();
    std::string dir;
    portable_getline(is, dir);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(a.get_work_directory()));
}

// ------------------------------------------------------------------------

static void
test_explicit_work_directory(void)
{
    bfs::path wdir = bfs::current_path() / "test.dir";

    bp::command_line cl(get_helpers_path());
    cl.argument("pwd");
    bp::attributes a(wdir.string());

    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        bp::launcher l(bp::launcher::REDIR_STDOUT);
        bp::child c = l.start(cl, a);

        bp::pistream& is = c.get_stdout();
        std::string dir;
        portable_getline(is, dir);

        bp::status s = c.wait();
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        BOOST_REQUIRE(s.exited());
        BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

        BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(a.get_work_directory()));
    } catch(...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}

// ------------------------------------------------------------------------

static void
test_unset_environment(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_UNSET");
    bp::attributes a;

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("TO_BE_UNSET", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_UNSET") != NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable("TO_BE_UNSET", "test") != 0);
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_UNSET", buf, 5) != 0);
#endif

    bp::launcher l(bp::launcher::REDIR_STDOUT);
    l.unset_environment("TO_BE_UNSET");
    bp::child c = l.start(cl, a);

    bp::pistream& is = c.get_stdout();
    std::string status;
    is >> status;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(status, "undefined");
}

// ------------------------------------------------------------------------

static void
test_set_environment(const std::string& value)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_SET");
    bp::attributes a;

#if defined(BOOST_PROCESS_POSIX_API)
    ::unsetenv("TO_BE_SET");
    BOOST_REQUIRE(::getenv("TO_BE_SET") == NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 0) == 0 ||
                  ::SetEnvironmentVariable("TO_BE_SET", NULL) != 0);
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 5) == 0);
#endif

    bp::launcher l(bp::launcher::REDIR_STDOUT);
    l.set_environment("TO_BE_SET", value);
    bp::child c = l.start(cl, a);

    bp::pistream& is = c.get_stdout();
    std::string status;
    is >> status;
    std::string gotval;
    is >> gotval;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(status, "defined");
    BOOST_CHECK_EQUAL(gotval, "'" + value + "'");
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

static void
test_shell(void)
{
#if defined(BOOST_PROCESS_POSIX_API)
    bp::command_line cl =
        bp::command_line::shell("echo test | sed 's,^,LINE-,'");
#elif defined(BOOST_PROCESS_WIN32_API)
    bp::command_line cl =
        bp::command_line::shell("if foo==foo echo LINE-test");
#endif
    bp::attributes a;
    bp::child c = bp::launcher(bp::launcher::REDIR_ALL).start(cl, a);

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
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("launcher test suite");

    test->add(BOOST_TEST_CASE(&test_flags), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_merge_out_err), 0, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_unset_environment), 0, 10);
#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_set_environment_empty), 0, 10);
#endif
    test->add(BOOST_TEST_CASE(&test_set_environment_non_empty), 0, 10);
    test->add(BOOST_TEST_CASE(&test_shell), 0, 10);

    return test;
}
