//
// Boost.Process
// Regression tests for the detail::command_line functions.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstddef>
#   include <cstring>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/process/command_line.hpp>
#include <boost/process/detail/command_line_ops.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_command_line_to_posix_argv(void)
{
    bp::command_line cl(get_helpers_path(), "program");
    cl.argument("arg1").argument("arg2").argument("arg3");

    std::pair< std::size_t, char** > args =
        bpd::command_line_to_posix_argv(cl);
    std::size_t argc = args.first;
    char** argv = args.second;

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
test_command_line_to_win32_cmdline(void)
{
    bp::command_line cl(get_helpers_path(), "program");
    cl.argument("arg1").argument("arg2").argument("arg3");

    boost::shared_array< TCHAR > cmdline =
        bpd::command_line_to_win32_cmdline(cl);
    BOOST_REQUIRE(::_tcscmp(cmdline.get(),
                            TEXT("program arg1 arg2 arg3")) == 0);
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test =
        BOOST_TEST_SUITE("detail::command_line_ops test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_command_line_to_posix_argv));
#elif defined(BOOST_PROCESS_WIN32_API)
    test->add(BOOST_TEST_CASE(&test_command_line_to_win32_cmdline));
#endif

    return test;
}
