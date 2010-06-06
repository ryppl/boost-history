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

#include <boost/all/process.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
    std::string exe = find_executable_in_path("hostname"); 
    context ctx; 
    ctx.stdout_behavior = capture; 
    child c = create_child(exe, ctx); 
    pistream &is = c.get_stdout(); 
    std::cout << is.rdbuf(); 
} 
