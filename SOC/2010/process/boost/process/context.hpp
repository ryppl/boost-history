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
#   include <unistd.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#endif 

#include <boost/process/stream_behavior.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/self.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace boost {
namespace process {

/**
 * Context class to define how a child process is created.
 *
 * The context class is used to configure standard streams and
 * to set the work directory and environment variables. It is also
 * used to change a process name (the variable commonly known as 
 * argv[0]).
 */
struct context
{
    /**
     * Behavior of the standard input stream.
     */
    boost::shared_ptr<behavior::stream> stdin_behavior;

    /**
     * Behavior of the standard output stream.
     */
    boost::shared_ptr<behavior::stream> stdout_behavior;

    /**
     * Behavior of the standard error stream.
     */
    boost::shared_ptr<behavior::stream> stderr_behavior;

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
    environment_t environment;

    /**
     * Constructs a process context.
     *
     * The default behavior of standard streams is to inherit them. The current
     * work directory is also the work directory of the child process. The child
     * process also inherits all environment variables.
     */
    context()
#if defined(BOOST_POSIX_API)
        : stdin_behavior(behavior::inherit::def(STDIN_FILENO)),
        stdout_behavior(behavior::inherit::def(STDOUT_FILENO)),
        stderr_behavior(behavior::inherit::def(STDERR_FILENO)),
#elif defined(BOOST_WINDOWS_API)
        : stdin_behavior(behavior::inherit::def(GetStdHandle(STD_INPUT_HANDLE))),
        stdout_behavior(behavior::inherit::def(GetStdHandle(STD_OUTPUT_HANDLE))),
        stderr_behavior(behavior::inherit::def(GetStdHandle(STD_ERROR_HANDLE))),
#endif
        work_dir(self::get_work_dir()),
        environment(self::get_environment())
    {
    }

#if defined(BOOST_PROCESS_DOXYGEN) || defined(BOOST_POSIX_API)
    /**
     * Setups a child process.
     *
     * This is an extension point to support more configuration options for
     * child processes. You override setup() in a user-defined context class
     * which should be derived from this class.
     *
     * On POSIX platforms setup() is called in the child process. On Windows
     * platforms setup() is called in the parent process. Furthermore a
     * reference to a STARTUPINFO structure is passed as a parameter on Windows
     * platforms.
     */
    void setup()
    {
    }
#elif defined(BOOST_WINDOWS_API)
    void setup(STARTUPINFOA &sainfo)
    {
    }
#endif
};

}
}

#endif
