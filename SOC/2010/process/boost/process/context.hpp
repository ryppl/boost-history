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
 * Includes the declaration of the context struct.
 *
 */

#include <boost/process/config.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/self.hpp>
#include <map>
#include <string>

#ifndef BOOST_PROCESS_CONTEXT_HPP
#define BOOST_PROCESS_CONTEXT_HPP

namespace boost {
namespace process {

/*
 * This is the declaration of context struct.
 *
 * The context struct defines a context for a processes
 * that will be created. It can defines the stream behaviors,
 * process name, the base directory for the process, and others.
 *
 * environment: It's a map from environment variables and it's values.
 * process_name: Define a name (argv[0]) of the process
 * io_service: TODO: Check when async is implemented
 * work_dir: The base dir for the new process.
 * stdin_behavior: Defines how the stdin of the process will be handled
 * stdout_behavior: Defines how the stdout of the process will be handled
 * stderr_behavior: ...
 *
 */
struct context
{
    environment_t environment;
    std::string process_name;
    std::string work_dir;
    stream_behavior stdin_behavior;
    stream_behavior stdout_behavior;
    stream_behavior stderr_behavior;

    context()
    {
        stdin_behavior = inherit;
        stdout_behavior = inherit;
        stderr_behavior = inherit;
        work_dir = self::get_work_dir();
        environment = self::get_environment();
    }
};

/*
 * This is the DEFALT_CONTEXT constant.
 * It represents a context with default values that will be
 * assign to a process if no context is passed by parameter.
 */
static struct context DEFAULT_CONTEXT;

}
}

#endif
