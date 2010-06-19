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

#include <boost/asio.hpp> 
#include <boost/process/all.hpp> 
#include <boost/array.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 
boost::array<char, 4096> buf; 

void handler(const boost::system::error_code &ec, std::size_t bytes_transferred); 

int main() 
{ 
    std::string exe = find_executable_in_path("hostname"); 
    context ctx; 
    ctx.stdout_behavior = behavior::named_pipe::def(behavior::named_pipe::output_stream); 
    child c = create_child(exe, ctx); 
    pistream &is = c.get_stdout(); 
    pipe read_end(ioservice, is.native().release()); 
    read_end.async_read_some(buffer(buf), handler); 
    ioservice.run(); 
} 

void handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{ 
    std::cout << std::string(buf.data(), bytes_transferred) << std::flush; 
} 
