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
#include <utility> 
#include <iostream> 
#include <unistd.h> 

//[file_descriptors_context 
std::pair<boost::process::handle, boost::process::handle> address = 
    boost::process::behavior::pipe()(false); 
std::pair<boost::process::handle, boost::process::handle> pid = 
    boost::process::behavior::pipe()(false); 

class context : public boost::process::context 
{ 
public: 
    void setup(std::vector<bool> &closeflags) 
    { 
        if (dup2(address.first.native(), 3) == -1) 
        { 
            write(STDERR_FILENO, "dup2() failed\n", 14); 
            _exit(127); 
        } 
        closeflags[3] = false; 

        if (dup2(pid.first.native(), 4) == -1) 
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
    address.first.close(); 
    pid.first.close(); 
    boost::process::pistream isaddress(address.second); 
    std::cout << isaddress.rdbuf() << std::endl; 
    boost::process::pistream ispid(pid.second); 
    std::cout << ispid.rdbuf() << std::endl; 
//] 
} 

#endif 
