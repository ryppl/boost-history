//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//
// This example program demonstrates multiple ways to modify a new child's
// environment variables.
//

// quickbook:begin(code)
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

namespace bp = ::boost::process;

//
// Helper function that forks a new process that shows the contents of
// the environment configured by the given launcher.
//
static
void
run_it(const std::string& msg, const bp::context& ctx)
{
    bp::context ctx2 = ctx;
    ctx2.m_stdout_behavior = bp::inherit_stream;
    ctx2.m_merge_stderr_with_stdout = true;

    std::cout << "===> " << msg << std::endl;
#if defined(BOOST_PROCESS_POSIX_API)
    std::vector< std::string > args;
    args.push_back("env");
    bp::status s =
        bp::launch(bp::find_executable_in_path("env"), args, ctx2).wait();
#elif defined(BOOST_PROCESS_WIN32_API)
    bp::status s = bp::launch_shell("set", ctx2).wait();
#endif
    if (s.exited() && s.exit_status() == EXIT_SUCCESS)
        std::cout << "     *** SUCCESS ***" << std::endl;
    else
        std::cout << "     *** FAILURE ***" << std::endl;
    std::cout << std::endl;
}

int
main(int argc, char* argv[])
{
    //
    // This first launcher does not touch the environment in the context
    // so it will be empty.
    //
    bp::context ctx1;
    run_it("Clean environment", ctx1);

    //
    // This second example obtains a snapshot of the current environment
    // and passes it down to the child process.
    //
    bp::context ctx2;
    ctx2.m_environment = bp::current_environment();
    run_it("Inherited environment", ctx2);

    //
    // This example adds an extra variable to the environment.
    //
    bp::context ctx3;
    ctx3.m_environment = bp::current_environment();
    ctx3.m_environment.insert
        (bp::environment::value_type("NEW_VARIABLE", "Hello, world!"));
    run_it("Environment with the NEW_VARIABLE extra variable", ctx3);

    //
    // This example removes a standard variable from the environment.
    //
    bp::context ctx4;
    ctx4.m_environment = bp::current_environment();
    ctx4.m_environment.erase("PATH");
    run_it("Environment without the standard PATH variable", ctx4);


    //
    // This last example illustrates how to start up a new child process
    // with a completely controlled environment table that is not subject
    // to existing variables at all.
    //
    bp::context ctx5;
    ctx5.m_environment.insert
        (bp::environment::value_type("HOME", "Known value for HOME"));
    ctx5.m_environment.insert
        (bp::environment::value_type("PATH", "Known value for PATH"));
    run_it("Completely controlled environment", ctx5);

    return EXIT_SUCCESS;
}
// quickbook:end(code)
