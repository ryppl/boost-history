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
#include <boost/bind.hpp> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 

void end_write(const boost::system::error_code &ec); 

int main() 
{ 
    parent p; 
    postream &os = p.get_stdin(); 
    pipe write_end(ioservice, os.native()); 
    async_write(write_end, buffer("Hello, world!"), boost::bind(&end_write, placeholders::error)); 
    ioservice.run(); 
} 

void end_write(const boost::system::error_code &ec) 
{ 
} 
