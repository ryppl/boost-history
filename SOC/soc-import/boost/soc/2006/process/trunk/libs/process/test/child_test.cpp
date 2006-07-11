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

#include <string>

#include <boost/process/child.hpp>
#include <boost/process/types.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

// XXX Extremely ugly way to determine helpers' path...
#if defined(__APPLE__)
#   define HELPERS_PATH "./bin/darwin/debug/helpers"
#else
#   define HELPERS_PATH "./bin/gcc/debug/helpers"
#endif

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

void
test_exit(const std::string& codename, int codenum)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("exit-" + codename);
    bp::attributes a(cl);

    bp::launcher l;
    bp::child c = l.start<bp::attributes>(a);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), codenum);
}

// ------------------------------------------------------------------------

void
test_exit_failure(void)
{
    test_exit("failure", EXIT_FAILURE);
}

// ------------------------------------------------------------------------

void
test_exit_success(void)
{
    test_exit("success", EXIT_SUCCESS);
}

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

    boost::weak_ptr< bp::postream > stin = c.get_input(bp::STDIN);
    boost::weak_ptr< bp::pistream > stout = c.get_output(bp::STDOUT);

    boost::shared_ptr< bp::postream > os;
    BOOST_REQUIRE(os = stin.lock());
    (*os) << "message-to-process" << std::endl;
    (*os).close();

    boost::shared_ptr< bp::pistream > is;
    BOOST_REQUIRE(is = stout.lock());
    std::string word;
    (*is) >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK(s.exit_status() == EXIT_SUCCESS);
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

    boost::weak_ptr< bp::pistream > st = c.get_output(desc);
    boost::shared_ptr< bp::pistream > is;
    BOOST_REQUIRE(is = st.lock());
    std::string word;
    (*is) >> word;
    BOOST_CHECK_EQUAL(word, expmsg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK(s.exit_status() == EXIT_SUCCESS);
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

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("child test suite");

    test->add(BOOST_TEST_CASE(&test_exit_success), 0, 10);
    test->add(BOOST_TEST_CASE(&test_exit_failure), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);

    return test;
}
