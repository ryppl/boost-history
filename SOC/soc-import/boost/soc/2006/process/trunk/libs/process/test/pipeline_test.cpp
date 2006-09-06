//
// Boost.Process
// Regression tests for the launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring>

#include <boost/filesystem/operations.hpp>
#include <boost/process/children.hpp>
#include <boost/process/pipeline.hpp>
#include <boost/test/unit_test.hpp>

#include "launcher_base_test.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

class start
{
public:
    bp::children
    operator()(bp::pipeline& p, const std::vector< std::string >& args,
               bool usein = false)
        const
    {
        std::vector< std::string > dummy;
        dummy.push_back("helpers");
        dummy.push_back("stdin-to-stdout");

        bp::pipeline p2 = p;
        if (usein)
            p2.add(get_helpers_path(), args).add(get_helpers_path(), dummy);
        else
            p2.add(get_helpers_path(), dummy).add(get_helpers_path(), args);
        return p2.start();
    }
};

// ------------------------------------------------------------------------

static
void
test_exit(const std::string& middle, int value)
{
    std::vector< std::string > args1;
    args1.push_back("helpers");
    args1.push_back("exit-success");

    std::vector< std::string > args2;
    args2.push_back("helpers");
    args2.push_back(middle);

    std::vector< std::string > args3;
    args3.push_back("helpers");
    args3.push_back("exit-success");

    bp::pipeline p;
    p.add(get_helpers_path(), args1);
    p.add(get_helpers_path(), args2);
    p.add(get_helpers_path(), args3);
    bp::children cs = p.start();

    BOOST_REQUIRE(cs.size() == 3);

    bp::status s = cs.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), value);
}

// ------------------------------------------------------------------------

static
void
test_exit_failure(void)
{
    test_exit("exit-failure", EXIT_FAILURE);
}

// ------------------------------------------------------------------------

static
void
test_exit_success(void)
{
    test_exit("exit-success", EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static
void
test_simple(void)
{
    std::vector< std::string > args1;
    args1.push_back("helpers");
    args1.push_back("prefix");
    args1.push_back("proc1-");

    std::vector< std::string > args2;
    args2.push_back("helpers");
    args2.push_back("prefix");
    args2.push_back("proc2-");

    bp::pipeline p;
    p.set_stdin_behavior(bp::redirect_stream);
    p.set_stdout_behavior(bp::redirect_stream);
    p.add(get_helpers_path(), args1);
    p.add(get_helpers_path(), args2);
    bp::children cs = p.start();

    BOOST_REQUIRE(cs.size() == 2);

    cs.get_stdin() << "message" << std::endl;
    cs.get_stdin().close();

    std::string word;
    cs.get_stdout() >> word;
    BOOST_CHECK_EQUAL(word, "proc2-proc1-message");

    bp::status s = cs.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static
void
test_merge_first(void)
{
    std::vector< std::string > args1;
    args1.push_back("helpers");
    args1.push_back("echo-stdout-stderr");
    args1.push_back("message");

    std::vector< std::string > args2;
    args2.push_back("helpers");
    args2.push_back("prefix");
    args2.push_back("second:");

    bp::pipeline p;
    p.set_stdout_behavior(bp::redirect_stream);
    p.add(get_helpers_path(), args1, true);
    p.add(get_helpers_path(), args2);
    bp::children cs = p.start();

    BOOST_REQUIRE(cs.size() == 2);

    std::string line;
    portable_getline(cs.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "second:stdout message");
    portable_getline(cs.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "second:stderr message");

    bp::status s = cs.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("pipeline test suite");

    add_tests_launcher_base< bp::pipeline, bp::children, start >(test);

    test->add(BOOST_TEST_CASE(&test_exit_success), 0, 10);
    test->add(BOOST_TEST_CASE(&test_exit_failure), 0, 10);
    test->add(BOOST_TEST_CASE(&test_simple), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge_first), 0, 10);

    return test;
}
