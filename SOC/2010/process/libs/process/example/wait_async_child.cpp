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
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 

void end_wait(const boost::system::error_code &ec, int exit_code); 

int main() 
{ 
    std::string exe = find_executable_in_path("hostname"); 
    child c = create_child(exe); 
    status s(ioservice); 
    s.async_wait(c.get_id(), end_wait);
    ioservice.run(); 
} 

void end_wait(const boost::system::error_code &ec, int exit_code) 
{ 
    if (!ec) 
        std::cout << "exit code: " << exit_code << std::endl; 
} 
