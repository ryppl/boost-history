//
// Boost.Process
// Regression tests for the posix_launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <cstring> // XXX For EXIT_* codes; should be hidden by the library.
#   include <string>

#   include <boost/process/posix_child.hpp>
#   include <boost/process/posix_launcher.hpp>

#   include "misc.hpp"

namespace bp = ::boost::process;
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
test_input(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("stdin-to-stdout");

    bp::posix_launcher l;
    l.redir_input(STDIN_FILENO).redir_output(STDOUT_FILENO);
    bp::posix_child c = l.start(cl);

    bp::postream& os = c.get_input(STDIN_FILENO);
    bp::pistream& is = c.get_output(STDOUT_FILENO);

    os << "message-to-process" << std::endl;
    os.close();

    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
test_output(int desc,
            const std::string& realmsg,
            const std::string& expmsg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("posix-echo-one").argument(desc).argument(realmsg);

    bp::posix_launcher l;
    l.redir_output(desc);
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(desc);
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, expmsg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_stderr_fail(void)
{
    test_output(STDERR_FILENO, "message-stderr", "fail-stderr");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_stderr_pass(void)
{
    test_output(STDERR_FILENO, "message-stderr", "message-stderr");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_stdout_fail(void)
{
    test_output(STDOUT_FILENO, "message-stdout", "fail-stdout");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_stdout_pass(void)
{
    test_output(STDOUT_FILENO, "message-stdout", "message-stdout");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
test_merge(int desc1,
           int desc2,
           const std::string& msg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("posix-echo-two").argument(desc1).argument(desc2);
    cl.argument(msg);

    bp::posix_launcher l;
    l.redir_output(desc1);
    l.merge_outputs(desc2, desc1);
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(desc1);
    int dtmp;
    std::string word;
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc1);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc2);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_merge_out_err(void)
{
    test_merge(STDOUT_FILENO, STDERR_FILENO, "message");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_merge_err_out(void)
{
    test_merge(STDERR_FILENO, STDOUT_FILENO, "message");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_merge_non_std(void)
{
    test_merge(4, 5, "message");
    test_merge(10, 20, "message");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_default_work_directory(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("pwd");

    bp::posix_launcher l;
    l.redir_output(STDOUT_FILENO);
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(STDOUT_FILENO);
    std::string dir;
    is >> dir;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(dir, l.get_work_directory());
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_explicit_work_directory(void)
{
    bfs::path wdir = bfs::current_path() / "test.dir";

    bp::command_line cl(get_helpers_path());
    cl.argument("pwd");

    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        bp::posix_launcher l;
        l.redir_output(STDOUT_FILENO);
        l.set_work_directory(wdir.string());
        bp::posix_child c = l.start(cl);

        bp::pistream& is = c.get_output(STDOUT_FILENO);
        std::string dir;
        is >> dir;

        bp::status s = c.wait();
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        BOOST_REQUIRE(s.exited());
        BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

        BOOST_CHECK_EQUAL(dir, l.get_work_directory());
    } catch (...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_unset_environment(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_UNSET");

    BOOST_REQUIRE(::setenv("TO_BE_UNSET", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_UNSET") != NULL);

    bp::posix_launcher l;
    l.redir_output(STDOUT_FILENO);
    l.unset_environment("TO_BE_UNSET");
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(STDOUT_FILENO);
    std::string status;
    is >> status;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(status, "undefined");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_set_environment(const std::string& value)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("query-env").argument("TO_BE_SET");

    ::unsetenv("TO_BE_SET");
    BOOST_REQUIRE(::getenv("TO_BE_SET") == NULL);

    bp::posix_launcher l;
    l.redir_output(STDOUT_FILENO);
    l.set_environment("TO_BE_SET", value);
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(STDOUT_FILENO);
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
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_set_environment_empty(void)
{
    test_set_environment("");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_set_environment_non_empty(void)
{
    test_set_environment("some-value");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_default_ids(void)
{
    bp::posix_launcher pl;
    BOOST_CHECK_EQUAL(pl.get_gid(), ::getgid());
    BOOST_CHECK_EQUAL(pl.get_egid(), ::getegid());
    BOOST_CHECK_EQUAL(pl.get_uid(), ::getuid());
    BOOST_CHECK_EQUAL(pl.get_euid(), ::geteuid());
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_setters(void)
{
    bp::posix_launcher pl1;
    gid_t gid = pl1.get_gid() + 1;
    pl1.set_gid(gid);
    BOOST_CHECK_EQUAL(pl1.get_gid(), gid);

    bp::posix_launcher pl2;
    gid_t egid = pl2.get_egid() + 1;
    pl2.set_egid(egid);
    BOOST_CHECK_EQUAL(pl2.get_egid(), egid);

    bp::posix_launcher pl3;
    uid_t uid = pl3.get_uid() + 1;
    pl3.set_uid(uid);
    BOOST_CHECK_EQUAL(pl3.get_uid(), uid);

    bp::posix_launcher pl4;
    uid_t euid = pl4.get_euid() + 1;
    pl4.set_euid(euid);
    BOOST_CHECK_EQUAL(pl4.get_euid(), euid);

    bp::posix_launcher pl5;
    pl5.set_chroot("/some/directory");
    BOOST_CHECK_EQUAL(pl5.get_chroot(), "/some/directory");
}
#endif

// ------------------------------------------------------------------------

#if !defined(BOOST_PROCESS_POSIX_API)
static
void
test_dummy(void)
{
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("posix_launcher test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_stdout_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_merge_out_err), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge_err_out), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge_non_std), 0, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_unset_environment), 0, 10);
    test->add(BOOST_TEST_CASE(&test_set_environment_empty), 0, 10);
    test->add(BOOST_TEST_CASE(&test_set_environment_non_empty), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_ids));
    test->add(BOOST_TEST_CASE(&test_setters));
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
