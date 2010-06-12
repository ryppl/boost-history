/
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
#include <boost/assign/list_of.hpp> 
#include <vector> 
#include <string> 
#include <utility> 

using namespace boost::process; 

int main() 
{ 
    child c1 = create_child(find_executable_in_path("hostname")); 

    std::vector<std::string> args = boost::assign::list_of("-?"); 
    child c2 = create_child(find_executable_in_path("hostname"), args); 

    context ctx; 
    ctx.environment.insert(std::make_pair("NEW_ENV_VARIABLE", "VALUE")); 
    
    ctx.stdin_behavior = boost::make_shared<close>(close());
    ctx.stdout_behavior = boost::make_shared<close>(close()); 
    child c3 = create_child(find_executable_in_path("hostname"), args, ctx); 

    int x;
    std::cin >> x;
} 
