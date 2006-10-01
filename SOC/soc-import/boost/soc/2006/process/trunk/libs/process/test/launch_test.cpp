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

#include <boost/filesystem/operations.hpp>
#include <boost/process/child.hpp>
#include <boost/process/context.hpp>
#include <boost/process/operations.hpp>
#include <boost/test/unit_test.hpp>

#include "launch_base_test.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

class launcher
{
public:
    bp::child
    operator()(const std::vector< std::string > args,
               bp::context ctx,
               bp::stream_behavior bstdin = bp::close_stream,
               bp::stream_behavior bstdout = bp::close_stream,
               bp::stream_behavior bstderr = bp::close_stream,
               bool merge_stderr_with_stdout = false,
               bool usein = false)
        const
    {
        ctx.m_stdin_behavior = bstdin;
        ctx.m_stdout_behavior = bstdout;
        ctx.m_stderr_behavior = bstderr;
        ctx.m_merge_stderr_with_stdout = merge_stderr_with_stdout;
        return bp::launch(get_helpers_path(), args, ctx);
    }
};

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("launch test suite");

    add_tests_launch_base< launcher, bp::context, bp::child >(test);

    return test;
}
