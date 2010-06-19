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
#include <boost/bind.hpp> 
#include <string> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 

void handler(const boost::system::error_code &ec, std::size_t bytes_transferred); 

int main() 
{ 
    std::string exe = find_executable_in_path("ftp"); 
    context ctx; 
    ctx.stdin_behavior = behavior::named_pipe::def(behavior::named_pipe::input_stream); 
    child c = create_child(exe, ctx); 
    postream &os = c.get_stdin(); 
    pipe write_end(ioservice, os.native().release()); 
    async_write(write_end, buffer("quit\n"), handler); 
    ioservice.run(); 
} 

void handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{ 
} 
