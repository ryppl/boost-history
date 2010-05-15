// write asynchronously to parent process (suppose this program is always started as a child process) 

#include <boost/all/process.hpp> 
#include <boost/asio.hpp> 
#include <boost/bind.hpp> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 

void end_write(const boost::system::error_code &ec); 

int main() 
{ 
  // ctx.io_service doesn't make much sense; or are there any other 
  // context settings to configure for a parent? 
  parent p(ioservice); 
  postream &os = p.get_stdin(); 
  async_write(os, buffer("Hello, world!"), boost::bind(&end_write, placeholders::error)); 
  ioservice.run(); 
} 

void end_write(const boost::system::error_code &ec) 
{ 
} 

