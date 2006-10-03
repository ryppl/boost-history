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
// This example program demonstrates how to interact with child processes
// under a POSIX system, taking advantage of features not available under
// all operating systems.  The program starts up a D-BUS daemon and
// shows how to read information from non-standard streams.
//

// Avoid building this example under non-POSIX systems.
#include <boost/process/config.hpp>
#if defined(BOOST_PROCESS_POSIX_API)

// quickbook:begin(code)
extern "C" {
#include <unistd.h>
}

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

//
// Error out early if we are trying to build this non-portable example
// code under a platform that does not provide the required posix_*
// classes.
//
#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    //
    // Constructs a command line to launch a new D-BUS session daemon.
    //
    std::string exe = bp::find_executable_in_path("dbus-daemon");
    std::vector< std::string > args;
    args.push_back(bp::executable_to_progname(exe));
    args.push_back("--fork");
    args.push_back("--session");

    //
    // The following arguments ask the dbus-daemon program to print the
    // new daemon's bind address and PID into two non-standard streams
    // (i.e. not stdout nor stderr).
    //
    args.push_back("--print-address=3");
    args.push_back("--print-pid=4");

    //
    // Constructs the launcher for the previous command line.  We ask
    // it to inherit our stdout and stderr for simplicity and we capture
    // the two non-standard streams into which the daemon will print the
    // communication information.
    //
    bp::posix_context ctx;
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(STDOUT_FILENO, bp::inherit_stream()));
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(STDERR_FILENO, bp::inherit_stream()));
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(3, bp::capture_stream()));
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(4, bp::capture_stream()));

    //
    // Spawns the child process.
    //
    bp::posix_child c = bp::posix_launch(exe, args, ctx);

    //
    // Reads the information printed by the dbus-daemon child process
    // from the two non-standard channels.
    //
    std::string address;
    pid_t pid;
    c.get_output(3) >> address;
    c.get_output(4) >> pid;

    //
    // Waits until the process exits and parses its termination status.
    //
    const bp::status s = c.wait();
    if (s.m_exit_status) {
        if (s.m_exit_status.get() == EXIT_SUCCESS) {
            std::cout << "D-BUS daemon's address is: " << address << std::endl;
            std::cout << "D-BUS daemon's PID is: " << pid << std::endl;
        } else
            std::cout << "D-BUS daemon returned error condition: "
                      << s.m_exit_status.get() << std::endl;
    } else {
        std::cout << "D-BUS daemon terminated abnormally" << std::endl;
    }

    return s.m_exit_status ? s.m_exit_status.get() : EXIT_FAILURE;
}
// quickbook:end(code)

#else // !defined(BOOST_PROCESS_POSIX_API)

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    std::cerr << "This example program is not supported in this platform."
              << std::endl;
    return EXIT_FAILURE;
}

#endif // defined(BOOST_PROCESS_POSIX_API)
