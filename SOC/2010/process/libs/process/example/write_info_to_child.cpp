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
#include <boost/foreach.hpp> 
#include <vector> 
#include <iterator> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
    std::vector<process> processes; 
    options opts; 
    opts.children_only = true; 
    create_snapshot(std::back_inserter(processes), opts); 

    BOOST_FOREACH(process &p, processes) 
        p.set_priority(HIGH); 
} 
