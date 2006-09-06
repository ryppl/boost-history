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

#include <cstdlib>
#include <string>
#include <vector>

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstddef>
#   include <cstring>
#   include <boost/process/detail/posix_ops.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <boost/process/detail/win32_ops.hpp>
#else
#   error "Unsupported platform."
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/process/child.hpp>
#include <boost/process/launcher.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
std::string
get_argument(const std::string& word)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("echo-quoted");
    args.push_back(word);

    bp::launcher l;
    l.set_stdout_behavior(bp::redirect_stream);

    bp::child c = l.start(get_helpers_path(), args);
    bp::pistream& is = c.get_stdout();

    std::string result;
    portable_getline(is, result);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);

    return result;
}

// ------------------------------------------------------------------------

static
void
test_quoting(void)
{
    BOOST_CHECK_EQUAL(get_argument("foo"), ">>>foo<<<");
    BOOST_CHECK_EQUAL(get_argument("foo "), ">>>foo <<<");
    BOOST_CHECK_EQUAL(get_argument(" foo"), ">>> foo<<<");
    BOOST_CHECK_EQUAL(get_argument("foo bar"), ">>>foo bar<<<");

    BOOST_CHECK_EQUAL(get_argument("foo\"bar"), ">>>foo\"bar<<<");
    BOOST_CHECK_EQUAL(get_argument("foo\"bar\""), ">>>foo\"bar\"<<<");
    BOOST_CHECK_EQUAL(get_argument("\"foo\"bar"), ">>>\"foo\"bar<<<");
    BOOST_CHECK_EQUAL(get_argument("\"foo bar\""), ">>>\"foo bar\"<<<");

    BOOST_CHECK_EQUAL(get_argument("*"), ">>>*<<<");
    BOOST_CHECK_EQUAL(get_argument("?*"), ">>>?*<<<");
    BOOST_CHECK_EQUAL(get_argument("[a-z]*"), ">>>[a-z]*<<<");
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_collection_to_posix_argv(void)
{
    std::vector< std::string > args;
    args.push_back("program");
    args.push_back("arg1");
    args.push_back("arg2");
    args.push_back("arg3");

    std::pair< std::size_t, char** > p = bpd::collection_to_posix_argv(args);
    std::size_t argc = p.first;
    char** argv = p.second;

    BOOST_REQUIRE_EQUAL(argc, static_cast< std::size_t >(4));

    BOOST_REQUIRE(std::strcmp(argv[0], "program") == 0);
    BOOST_REQUIRE(std::strcmp(argv[1], "arg1") == 0);
    BOOST_REQUIRE(std::strcmp(argv[2], "arg2") == 0);
    BOOST_REQUIRE(std::strcmp(argv[3], "arg3") == 0);
    BOOST_REQUIRE(argv[4] == NULL);

    delete [] argv[0];
    delete [] argv[1];
    delete [] argv[2];
    delete [] argv[3];
    delete [] argv;
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
static
void
test_collection_to_win32_cmdline(void)
{
    std::vector< std::string > args;
    args.push_back("program");
    args.push_back("arg1");
    args.push_back("arg2");
    args.push_back("arg3");

    boost::shared_array< TCHAR > cmdline =
        bpd::collection_to_win32_cmdline(args);
    BOOST_REQUIRE(::_tcscmp(cmdline.get(),
                            TEXT("program arg1 arg2 arg3")) == 0);
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("arguments test suite");

    test->add(BOOST_TEST_CASE(&test_quoting));

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_collection_to_posix_argv));
#elif defined(BOOST_PROCESS_WIN32_API)
    test->add(BOOST_TEST_CASE(&test_collection_to_win32_cmdline));
#endif

    return test;
}
