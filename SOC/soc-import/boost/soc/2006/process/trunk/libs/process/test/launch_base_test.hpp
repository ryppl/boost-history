//
// Boost.Process
// Common regression tests for Launcher implementations.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <string>
#include <utility>

#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;

// ------------------------------------------------------------------------

namespace launch_base_test {

// ------------------------------------------------------------------------

//
// Overview
// --------
//
// The functions below implement tests for common launcher functionality.
// These are used to test different implementations without duplicating
// much code.
//
// Launcher concept
// ----------------
//
// The functions in this file all rely on a Launcher concept.  This concept
// provides a class whose () operator starts a new process given an
// executable, its arguments, an execution context and the redirections for
// the standard streams, and returns a new Child object.  The operator also
// receives a boolean, defaulting to false, that indicates if the child
// process focuses on testing stdin input.  This is needed when testing
// pipelines to properly place a dummy process in the flow.
//
// I know this is ugly but allows reusing all tests to check various
// different launchers.  A nicer approach might be attempted later on.
//

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_close_stdin(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("is-closed-stdin");

    const bp::status s1 = Launcher()(args, Context(), bp::close_stream(),
                                     bp::close_stream(), bp::close_stream(),
                                     true).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    Child c2 = Launcher()(args, Context(), bp::capture_stream(),
                          bp::close_stream(), bp::close_stream(), true);
    c2.get_stdin() << "foo" << std::endl;
    c2.get_stdin().close();
    const bp::status s2 = c2.wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_close_stdout(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("is-closed-stdout");

    const bp::status s1 = Launcher()(args, Context()).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    const bp::status s2 = Launcher()(args, Context(), bp::close_stream(),
                                     bp::capture_stream()).wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_close_stderr(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("is-closed-stderr");

    const bp::status s1 = Launcher()(args, Context()).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    const bp::status s2 = Launcher()(args, Context(), bp::close_stream(),
                                     bp::close_stream(),
                               bp::capture_stream()).wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_input(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("stdin-to-stdout");

    Child c = Launcher()(args, Context(), bp::capture_stream(),
                         bp::capture_stream());

    bp::postream& os = c.get_stdin();
    bp::pistream& is = c.get_stdout();

    os << "message-to-process" << std::endl;
    os.close();

    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_output(bool out, const std::string& msg)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back(out ? "echo-stdout" : "echo-stderr");
    args.push_back(msg);

    Child c = Launcher()(args, Context(), bp::close_stream(),
                         out ? bp::capture_stream() : bp::close_stream(),
                         out ? bp::close_stream() : bp::capture_stream());

    std::string word;
    if (out) {
        bp::pistream& is = c.get_stdout();
        is >> word;
    } else {
        bp::pistream& is = c.get_stderr();
        is >> word;
    }
    BOOST_CHECK_EQUAL(word, msg);

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_stderr(void)
{
    test_output< Launcher, Context, Child >(false, "message1-stderr");
    test_output< Launcher, Context, Child >(false, "message2-stderr");
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_stdout(void)
{
    test_output< Launcher, Context, Child >(true, "message1-stdout");
    test_output< Launcher, Context, Child >(true, "message2-stdout");
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_redirect_err_to_out(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("echo-stdout-stderr");
    args.push_back("message-to-two-streams");

    Child c = Launcher()(args, Context(), bp::close_stream(),
                         bp::capture_stream(),
                         bp::redirect_stream_to_stdout());

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

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
check_work_directory(const std::string& wdir)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("pwd");

    Context ctx;
    if (wdir.empty())
        BOOST_CHECK(bfs::equivalent(ctx.m_work_directory,
                                    bfs::current_path().string()));
    else
        ctx.m_work_directory = wdir;
    Child c = Launcher()(args, ctx, bp::close_stream(),
                         bp::capture_stream());

    bp::pistream& is = c.get_stdout();
    std::string dir;
    portable_getline(is, dir);

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(ctx.m_work_directory));
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_work_directory(void)
{
    check_work_directory< Launcher, Context, Child >("");

    bfs::path wdir = bfs::current_path() / "test.dir";
    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        check_work_directory< Launcher, Context, Child >(wdir.string());
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
    } catch(...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
std::pair< bool, std::string >
get_var_value(Context& ctx, const std::string& var)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("query-env");
    args.push_back("TO_BE_SET");

    Child c = Launcher()(args, ctx, bp::close_stream(),
                         bp::capture_stream());

    bp::pistream& is = c.get_stdout();
    std::string status;
    is >> status;
    std::string gotval;
    if (status == "defined")
        is >> gotval;

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    return std::pair< bool, std::string >(status == "defined", gotval);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
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

    Context ctx;
    BOOST_REQUIRE_EQUAL(ctx.m_environment.size(),
                        (bp::environment::size_type)0);

    std::pair< bool, std::string > p1 =
        get_var_value< Launcher, Context, Child >(ctx, "TO_BE_QUERIED");
    BOOST_REQUIRE(!p1.first);

    std::pair< bool, std::string > p2 =
        get_var_value< Launcher, Context, Child >(ctx, "PATH");
    BOOST_REQUIRE(!p2.first);

    std::pair< bool, std::string > p3 =
        get_var_value< Launcher, Context, Child >(ctx, "HOME");
    BOOST_REQUIRE(!p3.first);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_unset_environment(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("query-env");
    args.push_back("TO_BE_UNSET");

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("TO_BE_UNSET", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_UNSET") != NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable("TO_BE_UNSET", "test") != 0);
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_UNSET", buf, 5) != 0);
#endif

    Context ctx;
    ctx.m_environment = bp::current_environment();
    ctx.m_environment.erase("TO_BE_UNSET");
    std::pair< bool, std::string > p =
        get_var_value< Launcher, Context, Child >(ctx, "TO_BE_SET");
    BOOST_CHECK(!p.first);
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_set_environment_var(const std::string& value)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("query-env");
    args.push_back("TO_BE_SET");

#if defined(BOOST_PROCESS_POSIX_API)
    ::unsetenv("TO_BE_SET");
    BOOST_REQUIRE(::getenv("TO_BE_SET") == NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 0) == 0 ||
                  ::SetEnvironmentVariable("TO_BE_SET", NULL) != 0);
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 5) == 0);
#endif

    Context ctx;
    ctx.m_environment.insert(bp::environment::value_type("TO_BE_SET", value));
    std::pair< bool, std::string > p =
        get_var_value< Launcher, Context, Child >(ctx, "TO_BE_SET");
    BOOST_CHECK(p.first);
    BOOST_CHECK_EQUAL(p.second, "'" + value + "'");
}

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
static
void
test_set_environment(void)
{
    if (bp_api_type == posix_api)
        test_set_environment_var< Launcher, Context, Child >("");
    test_set_environment_var< Launcher, Context, Child >("some-value-1");
    test_set_environment_var< Launcher, Context, Child >("some-value-2");
}

// ------------------------------------------------------------------------

} // namespace launch_base_test

// ------------------------------------------------------------------------

template< class Launcher, class Context, class Child >
void
add_tests_launch_base(boost::unit_test::test_suite* ts)
{
    using namespace launch_base_test;

    ts->add(BOOST_TEST_CASE
            (&(test_close_stdin< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_close_stdout< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_close_stderr< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_stdout< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_stderr< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_redirect_err_to_out< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_input< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_work_directory< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_clear_environment< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_unset_environment< Launcher, Context, Child >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_set_environment< Launcher, Context, Child >)), 0, 10);
}
