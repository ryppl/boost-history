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
#include <boost/asio.hpp> 
#include <boost/array.hpp> 
#include <boost/bind.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 
boost::array<char, 4096> buf; 

void begin_read(pipe &read_end); 
void end_read(const boost::system::error_code &ec, std::size_t bytes_transferred, pipe &read_end); 

int main() 
{ 
    std::string exe = find_executable_in_path("hostname"); 
    context ctx; 
    ctx.stdout_behavior = capture; 
    child c = create_child(exe, ctx); 
    pistream &is = c.get_stdout(); 
    pipe read_end(ioservice, is.native()); 
    begin_read(read_end); 
    ioservice.run(); 
} 

void begin_read(pipe &read_end) 
{ 
    read_end.async_read_some(buffer(buf), boost::bind(&end_read, placeholders::error, placeholders::bytes_transferred, read_end)); 
} 

void end_read(const boost::system::error_code &ec, std::size_t bytes_transferred, pipe &read_end) 
{ 
    if (!ec) 
    { 
      std::cout << std::string(buf.data(), bytes_transferred) << std::flush; 
      begin_read(read_end); 
    } 
} 
