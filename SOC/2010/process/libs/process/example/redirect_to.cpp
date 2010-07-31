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

#include <boost/shared_ptr.hpp> 
#include <boost/make_shared.hpp> 
#include <iostream> 
#include <stdexcept> 

//[redirect_to_stream 
class redirect_to : public boost::process::behavior::stream 
{ 
public: 
    redirect_to(boost::process::handle child_end) 
    { 
#if defined(BOOST_POSIX_API) 
        child_end_ = dup(child_end.native()); 
        if (!child_end_.valid()) 
            throw std::runtime_error("dup(2) failed"); 
#elif defined(BOOST_WINDOWS_API) 
        HANDLE h;
        if (!DuplicateHandle(GetCurrentProcess(), child_end.native(), 
            GetCurrentProcess(), &h, 0, TRUE, DUPLICATE_SAME_ACCESS)) 
            throw std::runtime_error("DuplicateHandle() failed"); 
        child_end_ = h; 
#endif 
    } 

    static boost::shared_ptr<redirect_to> create( 
        boost::process::handle stream_end) 
    { 
        return boost::make_shared<redirect_to>(redirect_to(stream_end)); 
    } 

    boost::process::handle get_child_end() 
    { 
        return child_end_; 
    } 

private: 
    boost::process::handle child_end_; 
}; 
//] 

//[redirect_to_main 
int main() 
{ 
    std::string executable = boost::process::find_executable_in_path( 
        "hostname"); 

    std::vector<std::string> args; 

    boost::process::context ctx; 
    ctx.stdout_behavior = boost::process::behavior::pipe::create( 
        boost::process::behavior::pipe::output_stream); 
    ctx.stderr_behavior = redirect_to::create( 
        ctx.stdout_behavior->get_child_end()); 

    boost::process::child c = boost::process::create_child( 
        executable, args, ctx); 

    boost::process::pistream &is = c.get_stdout(); 
    std::cout << is.rdbuf() << std::flush; 
} 
//] 
