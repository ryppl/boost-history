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

#include <boost/process/all.hpp> 
#include <iostream> 
#include <vector> 
#include <string> 
#include <utility> 

using namespace boost::process; 

int main() 
{ 
    child c1 = create_child(find_executable_in_path("hostname")); 

    std::vector<std::string> args; 
    args.push_back("-?"); 
    child c2 = create_child(find_executable_in_path("hostname"), args); 

    context ctx; 
    ctx.environment.insert(std::make_pair("NEW_ENV_VARIABLE", "VALUE")); 
    ctx.process_name = "My_process_name"; 
    ctx.stdin_behavior = stream_behavior::inherit; 
    ctx.stdout_behavior = stream_behavior::closed; 
    ctx.stderr_behavior = stream_behavior::closed; 
    child c3 = create_child(find_executable_in_path("hostname"), args, ctx); 
} 
