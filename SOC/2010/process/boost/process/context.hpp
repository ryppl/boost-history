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

#include <boost/process/stream_behavior.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/self.hpp>

namespace boost {
namespace process {

/*
 * Context class that defines how a process is created. 
 */
struct context
{
    stream_behavior stdin_behavior;
    stream_behavior stdout_behavior;
    stream_behavior stderr_behavior;
    std::string process_name;
    std::string work_dir;
    environment_t environment;

    /** 
     * Constructs a process context. 
     * 
     * Sets behavior of standard streams (inherit), current work directory 
     * and environment variables. 
     */ 
    context() 
        : stdin_behavior(inherit), 
        stdout_behavior(inherit), 
        stderr_behavior(inherit), 
        work_dir(self::get_work_dir()), 
        environment(self::get_environment()) 
    {
    }
};

/** 
 * Default context object. 
 * 
 * The default context object is used when child processes are created 
 * without defining a context explicitly. 
 */
const context default_context; 

}
}

#endif
