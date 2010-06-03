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
#include <boost/assign/list_of.hpp> 
#include <string> 
#include <utility> 

using namespace boost::process; 

int main() 
{ 
    process_factory f("C:\\Windows\\notepad.exe"); 
    f.environment.insert(std::make_pair("NEW_ENV_VARIABLE", "VALUE")); 
    child = f.create(); 
} 
