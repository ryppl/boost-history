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
#include <boost/asio.hpp> 
#include <boost/bind.hpp> 
#include <vector> 
#include <iterator> 
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 
status s(ioservice); 

void end_wait(const boost::system::error_code &ec); 

int main() 
{ 
    std::vector<process> processes; 
    create_snapshot(std::back_inserter(processes)); 
    process p = processes.front(); 
    status &s = p.status(ioservice); 
    s.async_wait(boost::bind(&end_wait, placeholders::error)); 
    ioservice.run(); 
} 

void end_wait(const boost::system::error_code &ec) 
{ 
    if (!ec) 
        std::cout << "process exited" << std::endl; 
} 
