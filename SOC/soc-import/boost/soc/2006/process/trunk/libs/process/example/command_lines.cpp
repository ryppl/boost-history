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
// This example program demonstrates multiple ways to construct a command
// line and why such cases can be useful.
//

//[command_lines_all
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

namespace bp = ::boost::process;

//
// Helper function that executes the given executable/arguments pair.
//
static
void
run(const std::string& exec, const std::vector< std::string >& args)
{
    std::cout << "===> Binary:       " << exec << std::endl;
    std::cout << "===> Command line: ";
    for (std::vector< std::string >::size_type i = 0; i < args.size(); i++) {
        std::cout << args[i];
        if (i < args.size() - 1)
            std::cout << ' ';
    }
    std::cout << std::endl;

    bp::context ctx;
    ctx.m_stdout_behavior = bp::inherit_stream();
    ctx.m_stderr_behavior = bp::inherit_stream();

    const bp::status s = bp::launch(exec, args, ctx).wait();
    if (s.exited() && s.exit_status() == EXIT_SUCCESS)
        std::cout << "     *** SUCCESS ***" << std::endl;
    else
        std::cout << "     *** FAILURE ***" << std::endl;
    std::cout << std::endl;
}

//
// Helper function that executes the given command through the system shell.
//
static
void
run(const std::string& command)
{
    std::cout << "===> Shell command: " << command << std::endl;

    bp::context ctx;
    ctx.m_stdout_behavior = bp::inherit_stream();
    ctx.m_stderr_behavior = bp::inherit_stream();
    ctx.m_environment = bp::current_environment();

    const bp::status s = bp::launch_shell(command, ctx).wait();
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
    // Executes the 'ls' binary to list the contents of the current directory.
    //
    std::string exec1 = bp::find_executable_in_path("ls");
    std::vector< std::string > args1;
    args1.push_back("ls");
    run(exec1, args1);

    //
    // Executes the 'ls' binary again but this time passes an argument to the
    // binary telling it which directory to list.
    //
    std::string exec2 = bp::find_executable_in_path("ls");
    std::vector< std::string > args2;
    args2.push_back("ls");
    args2.push_back("/tmp");
    run(exec2, args2);

    //
    // Executes another system binary, but this time with three arguments.
    // Note that when calling '/bin/test' with its default program name
    // 'test', the closing square bracket is not allowed.
    //
    std::string exec3 = bp::find_executable_in_path("test");
    std::vector< std::string > args3;
    args3.push_back("test");
    args3.push_back("foo");
    args3.push_back("=");
    args3.push_back("foo");
    run(exec3, args3);

    //
    // Exactly the same command as the previous one but this one changes
    // the program name at execution.  Here, '/bin/test' is executed with
    // a program name of '[' (i.e. argv[0]'s value is "[").  Hence, it
    // requires a closing square bracket as an extra argument to execute
    // properly.
    //
    // XXX Not portable!?
    //
    std::string exec4 = bp::find_executable_in_path("test");
    std::vector< std::string > args4;
    args4.push_back("[");
    args4.push_back("foo");
    args4.push_back("=");
    args4.push_back("foo");
    args4.push_back("]");
    run(exec4, args4);

    //
    // Executes a command through the default system shell, much like the
    // ::system(3) function does.  These calls are much simpler to write
    // and read because the command line is provided as a single string
    // instead of a collection.  However, they may be subject to quoting
    // problems because they need to be run through the shell.  Also note
    // that there is no need to give the full path to the binary because
    // the shell will resolve it for us.
    //
    run("ls -l /var");

    //
    // Another shell command line construction used to demonstrate that
    // the command is really executed through the system shell.  Otherwise
    // the stdout redirection could not work.
    //
    run("echo foo > /dev/null");

    return EXIT_SUCCESS;
}
//]
