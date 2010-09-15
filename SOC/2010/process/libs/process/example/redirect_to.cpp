// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling 
// Copyright (c) 2009 Boris Schaeling 
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling 
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

#include <boost/process/all.hpp> 

#if defined(BOOST_POSIX_API) 
#   include <unistd.h> 
#elif defined(BOOST_WINDOWS_API) 
#   include <windows.h> 
#else 
#   error "Unsupported platform." 
#endif 

#include <boost/bind.hpp> 
#include <iostream> 
#include <utility> 
#include <stdexcept> 

//[redirect_to_stream 
boost::process::stream_ends redirect_to(boost::process::handle h) 
{ 
#if defined(BOOST_WINDOWS_API) 
    if (!SetHandleInformation(h.native(), HANDLE_FLAG_INHERIT, 
        HANDLE_FLAG_INHERIT)) 
        throw std::runtime_error("DuplicateHandle() failed"); 
#endif 
    return boost::process::stream_ends(h, boost::process::handle()); 
} 
//] 

//[redirect_to_main 
boost::process::stream_ends forward(boost::process::stream_ends ends) 
{ 
    return ends; 
} 

int main() 
{ 
    std::string executable = boost::process::find_executable_in_path( 
        "hostname"); 

    std::vector<std::string> args; 

    boost::process::stream_ends ends = boost::process::behavior::pipe()(false); 

    boost::process::context ctx; 
    ctx.stdout_behavior = boost::bind(forward, ends); 
    ctx.stderr_behavior = boost::bind(redirect_to, ends.child); 

    boost::process::child c = boost::process::create_child( 
        executable, args, ctx); 

    boost::process::pistream &is = c.get_stdout(); 
    std::cout << is.rdbuf() << std::flush; 
} 
//] 
