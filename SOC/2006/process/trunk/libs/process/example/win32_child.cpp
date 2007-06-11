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
// This example program demonstrates how to start a process in a Win32
// operating system and later retrieve handles and identifiers to both
// the process and its primary thread.
//

// Avoid building this example under non-Win32 systems.
#include <boost/process/config.hpp>
#if defined(BOOST_PROCESS_WIN32_API)

//[win32_child_all
extern "C" {
#include <windows.h>
}

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

//
// Error out early if we are trying to build this non-portable example
// code under a platform that does not provide the required win32_*
// classes.
//
#if !defined(BOOST_PROCESS_WIN32_API)
#   error "Unsupported platform."
#endif

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    int exitstatus = EXIT_FAILURE;

    try {
        //
        // Constructs a command line to launch Notepad, looking for its
        // availability in the PATH.
        //
        std::string exe = bp::find_executable_in_path("notepad");
        std::vector< std::string > args;
        args.push_back("notepad");

        //
        // Starts the process.
        //
        bp::win32_child c = bp::win32_launch(exe, args, bp::win32_context());

        //
        // Prints out information about the new process.  Note that,
        // except for the process handle, the other information is only
        // available because we are using the win32_child.
        //
        std::cout << "Process handle            : 0x"
                  << c.get_handle() << std::endl;
        std::cout << "Process identifier        : "
                  << c.get_id() << std::endl;
        std::cout << "Primary thread handle     : 0x"
                  << c.get_primary_thread_handle() << std::endl;
        std::cout << "Primary thread identifier : "
                  << c.get_primary_thread_id() << std::endl;

        //
        // Waits until the process terminates and reports status.
        //
        const bp::status s = c.wait();
        if (s.exited()) {
            std::cout << "Application exited successfully" << std::endl;
            exitstatus = EXIT_SUCCESS;
        } else {
            std::cout << "The application returned an error" << std::endl;
        }
    } catch (bp::not_found_error< std::string > e) {
        std::cout << "Could not find notepad in path." << std::endl;
    }

    return exitstatus;
}
//]

#else // !defined(BOOST_PROCESS_WIN32_API)

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    std::cerr << "This example program is not supported in this platform."
              << std::endl;
    return EXIT_FAILURE;
}

#endif // defined(BOOST_PROCESS_WIN32_API)
