//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/context.hpp
 *
 * Includes the declaration of the context class.
 */

#ifndef BOOST_PROCESS_CONTEXT_HPP
#define BOOST_PROCESS_CONTEXT_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <vector>
#   include <unistd.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#endif

#include <boost/process/handle.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/self.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/function.hpp>
#include <string>
#include <utility>

namespace boost {
namespace process {

/**
 * Context class to define how a child process is created.
 *
 * The context class is used to configure standard streams and to set the work
 * directory and environment variables. It is also used to change a process
 * name (the variable commonly known as argv[0]).
 */
struct context
{
    /**
     * Behavior of the standard input stream.
     */
    boost::function<std::pair<handle, handle> (bool)> stdin_behavior;

    /**
     * Behavior of the standard output stream.
     */
    boost::function<std::pair<handle, handle> (bool)> stdout_behavior;

    /**
     * Behavior of the standard error stream.
     */
    boost::function<std::pair<handle, handle> (bool)> stderr_behavior;

    /**
     * Process name.
     *
     * The child process can access the process name via a variable
     * commonly known as argv[0].
     */
    std::string process_name;

    /**
     * Work directory.
     */
    std::string work_dir;

    /**
     * Environment variables.
     */
    environment env;

    /**
     * Constructs a process context.
     *
     * The default behavior of standard streams is to inherit them. The current
     * work directory is also the work directory of the child process. The child
     * process also inherits all environment variables.
     */
    context()
#if defined(BOOST_POSIX_API)
        : stdin_behavior(behavior::inherit(STDIN_FILENO)),
        stdout_behavior(behavior::inherit(STDOUT_FILENO)),
        stderr_behavior(behavior::inherit(STDERR_FILENO)),
#elif defined(BOOST_WINDOWS_API)
        : stdin_behavior(behavior::inherit(GetStdHandle(STD_INPUT_HANDLE))),
        stdout_behavior(behavior::inherit(GetStdHandle(STD_OUTPUT_HANDLE))),
        stderr_behavior(behavior::inherit(GetStdHandle(STD_ERROR_HANDLE))),
#endif
        work_dir(self::get_work_dir()),
        env(self::get_environment())
    {
    }

#if defined(BOOST_PROCESS_DOXYGEN)
    /**
     * Setups a child process.
     *
     * This is an extension point to support more configuration options for
     * child processes. You override setup() in a user-defined context class
     * which should be derived from this class.
     *
     * On POSIX platforms setup() is called in the child process. That's why in
     * a multithreaded application only async-signal-safe functions must be
     * called in setup(). A reference to a std::vector<bool> is passed which
     * is used to decide which file descriptors to close in the child process.
     * While the std::vector<bool> is automatically setup for standard streams
     * developers can change flags to avoid the library closing other file
     * descriptors.
     *
     * On Windows platforms setup() is called in the parent process. A
     * reference to a STARTUPINFOA structure is passed as parameter.
     */
    void setup()
    {
    }
#elif defined(BOOST_POSIX_API)
    void setup(std::vector<bool> &closeflags)
    {
    }
#elif defined(BOOST_WINDOWS_API)
    void setup(STARTUPINFOA &sainfo)
    {
#if defined(BOOST_MSVC)
        sainfo;
#endif
    }
#endif
};

}
}

#endif
