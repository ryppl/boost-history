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
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace boost {
namespace process {

/*
 * Context class that defines how a process is created. 
 */
struct context
{
    boost::shared_ptr<stream_behavior> stdin_behavior;
    boost::shared_ptr<stream_behavior> stdout_behavior;
    boost::shared_ptr<stream_behavior> stderr_behavior;
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
#if defined(BOOST_POSIX_API) 
        : stdin_behavior(boost::make_shared<inherit>(inherit(STDIN_FILENO))), 
        stdout_behavior(boost::make_shared<inherit>(inherit(STDOUT_FILENO))), 
        stderr_behavior(boost::make_shared<inherit>(inherit(STDERR_FILENO))), 
#elif defined(BOOST_WINDOWS_API) 
        : stdin_behavior(boost::make_shared<inherit>(inherit(::GetStdHandle(STD_INPUT_HANDLE)))), 
        stdout_behavior(boost::make_shared<inherit>(inherit(::GetStdHandle(STD_OUTPUT_HANDLE)))), 
        stderr_behavior(boost::make_shared<inherit>(inherit(::GetStdHandle(STD_ERROR_HANDLE)))), 
#endif 
        work_dir(self::get_work_dir()), 
        environment(self::get_environment()) 
    {
    }
};

}
}

#endif
