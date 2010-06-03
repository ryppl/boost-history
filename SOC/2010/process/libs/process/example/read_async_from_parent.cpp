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
    parent p(ioservice); 
    std::string buffer; 
    pistream &is = p.get_stdout(); 
    async_read(os, &buffer, boost::bind(&end_read, placeholders::error)); 
    ioservice.run(); 
} 

void end_read(const boost::system::error_code &ec) 
{ 
} 
