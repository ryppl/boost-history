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
// This example program demonstrates the very basic usage of Boost.Process.
// It takes a list of CVS work directories as arguments and iterates over
// them bringing each one up to date.
//

//[generic_all
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Please specify some directories." << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        std::cout << "===> Updating directory " << argv[i] << std::endl;

        //
        // Constructs the launcher.
        //
        // This launcher will redirect us CVS' stdout and stderr data flows
        // through its stdout stream.  This will allow us to print the
        // output in a fancy way.
        //
        // The launcher also forces the CVS client to use the Secure Shell
        // to connect to servers and unsets the CVSROOT environment
        // variable to avoid side-effects.
        //
        bp::context ctx;
        ctx.m_stdout_behavior = bp::capture_stream();
        ctx.m_stderr_behavior = bp::redirect_stream_to_stdout();
        ctx.m_work_directory = argv[i];
        ctx.m_environment = bp::current_environment();
        ctx.m_environment.insert
            (bp::environment::value_type("CVS_RSH", "ssh"));
        ctx.m_environment.erase("CVSROOT");

        //
        // Spawns the CVS child process.
        //
        // This command line executes a CVS process passing it an update
        // command.  Note that it will be executed through the standard system
        // shell so that it is easier for us to provide the command.
        //
        // In this case we have a command line that will work both on POSIX
        // systems and under Windows.
        //
        bp::child c = bp::launch_shell("cvs -z3 update -dP", ctx);

        //
        // Gets CVS' stdout stream.  As we asked for a merge of stdout and
        // stderr, this stream gets both outputs at once.
        //
        bp::pistream& is = c.get_stdout();

        //
        // Reads the child's output.
        //
        // As long as the child process (CVS) does not terminate, its
        // stdout will remain open and we will be able to retrieve data
        // from it.  Once it is closed, the 'is' stream will become invalid
        // and it will not return more data.
        //
        std::string line;
        while (std::getline(is, line))
            std::cout << "=> " << line << std::endl;

        //
        // Waits for the process to terminate and parses its exit status to
        // give the user feedback on the final results.
        //
        const bp::status s = c.wait();
        if (s.exited() && s.exit_status() == EXIT_SUCCESS)
            std::cout << "     *** SUCCESSFUL UPDATE ***" << std::endl;
        else
            std::cout << "     *** FAILED UPDATE ***" << std::endl;
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
//]
