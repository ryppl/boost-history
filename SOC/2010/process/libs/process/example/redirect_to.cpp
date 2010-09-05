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
std::pair<boost::process::handle, boost::process::handle> redirect_to(
    boost::process::handle h) 
{ 
#if defined(BOOST_POSIX_API) 
    h = dup(h.native()); 
    if (!h.valid()) 
        throw std::runtime_error("dup(2) failed"); 
#elif defined(BOOST_WINDOWS_API) 
    HANDLE h2; 
    if (!DuplicateHandle(GetCurrentProcess(), h.native(), 
        GetCurrentProcess(), &h2, 0, TRUE, DUPLICATE_SAME_ACCESS)) 
        throw std::runtime_error("DuplicateHandle() failed"); 
    h = h2; 
#endif 
    return std::make_pair(h, boost::process::handle()); 
} 
//] 

//[redirect_to_main 
std::pair<boost::process::handle, boost::process::handle> forward(
    std::pair<boost::process::handle, boost::process::handle> p) 
{ 
    return p; 
} 

int main() 
{ 
    std::string executable = boost::process::find_executable_in_path( 
        "hostname"); 

    std::vector<std::string> args; 

    std::pair<boost::process::handle, boost::process::handle> p = 
        boost::process::behavior::pipe()(false); 

    boost::process::context ctx; 
    ctx.stdout_behavior = boost::bind(forward, p); 
    ctx.stderr_behavior = boost::bind(redirect_to, p.first); 

    boost::process::child c = boost::process::create_child( 
        executable, args, ctx); 

    boost::process::pistream &is = c.get_stdout(); 
    std::cout << is.rdbuf() << std::flush; 
} 
//] 
