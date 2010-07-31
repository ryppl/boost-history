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

#include <boost/assign/list_of.hpp> 
#include <string> 
#include <vector> 
#include <iostream> 
#include <unistd.h> 

//[file_descriptors_context 
boost::process::behavior::pipe address( 
    boost::process::behavior::pipe::output_stream); 
boost::process::behavior::pipe pid( 
    boost::process::behavior::pipe::output_stream); 

class context : public boost::process::context 
{ 
public: 
    void setup(std::vector<bool> &closeflags) 
    { 
        if (dup2(address.get_child_end().native(), 3) == -1) 
        { 
            write(STDERR_FILENO, "dup2() failed\n", 14); 
            _exit(127); 
        } 
        closeflags[3] = false; 

        if (dup2(pid.get_child_end().native(), 4) == -1) 
        { 
            write(STDERR_FILENO, "dup2() failed\n", 14); 
            _exit(127); 
        } 
        closeflags[4] = false; 
    } 
}; 
//] 

int main() 
{ 
//[file_descriptors_main 
    std::string exe = boost::process::find_executable_in_path("dbus-daemon"); 
    std::vector<std::string> args = boost::assign::list_of("--fork")
        ("--session")("--print-address=3")("--print-pid=4"); 
    context ctx; 
    boost::process::create_child(exe, args, ctx); 
    address.get_child_end().close(); 
    pid.get_child_end().close(); 
    boost::process::pistream isaddress(address.get_parent_end()); 
    std::cout << isaddress.rdbuf() << std::endl; 
    boost::process::pistream ispid(pid.get_parent_end()); 
    std::cout << ispid.rdbuf() << std::endl; 
//] 
} 

#endif 
