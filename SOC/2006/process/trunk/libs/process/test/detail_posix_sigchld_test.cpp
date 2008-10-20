//
// Boost.Process
// Regression tests for the detail::posix_sigchld class.
//
// Copyright (c) 2008 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <sys/types.h>

#   include <signal.h>
#   include <unistd.h>

#   include <boost/process/detail/posix_sigchld.hpp>

namespace bpd = ::boost::process::detail;
#endif

#include <boost/test/unit_test.hpp>

namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
bool received = false;
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
sigchld_handler(int signo)
{
    received = true;
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
install_sigchld_handler(void)
{
    received = false;

    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    BOOST_CHECK(::sigaction(SIGCHLD, &sa, NULL) != -1);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_test_handler(void)
{
    install_sigchld_handler();
    BOOST_CHECK(!received);
    BOOST_CHECK(::kill(::getpid(), SIGCHLD) != -1);
    BOOST_CHECK(received);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_fork_fail(void)
{
    install_sigchld_handler();

    bpd::the_posix_sigchld_programmer.forking_child();
    BOOST_CHECK(::kill(::getpid(), SIGCHLD) != -1);
    BOOST_CHECK(!received);

    bpd::the_posix_sigchld_programmer.forking_failed();
    BOOST_CHECK(::kill(::getpid(), SIGCHLD) != -1);
    BOOST_CHECK(received);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_fork_wait(void)
{
    install_sigchld_handler();

    bpd::the_posix_sigchld_programmer.forking_child();
    BOOST_CHECK(::kill(::getpid(), SIGCHLD) != -1);
    BOOST_CHECK(!received);

    bpd::the_posix_sigchld_programmer.child_awaited();
    BOOST_CHECK(::kill(::getpid(), SIGCHLD) != -1);
    BOOST_CHECK(received);
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
    but::test_suite* test = BOOST_TEST_SUITE("posix_child test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_test_handler));
    test->add(BOOST_TEST_CASE(&test_fork_fail));
    test->add(BOOST_TEST_CASE(&test_fork_wait));
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
