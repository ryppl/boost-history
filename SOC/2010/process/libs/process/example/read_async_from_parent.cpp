// read asynchronously to parent process (suppose this program is always started as a child process) 

#include <boost/all/process.hpp> 
#include <boost/asio.hpp> 
#include <boost/bind.hpp> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 

void end_write(const boost::system::error_code &ec); 

int main() 
{ 
  // Boris: ctx.io_service doesn't make much sense; or are there any other 
  // context settings to configure for a parent? 
  //
  // Felipe: Maybe as a member of parent?

  parent p(ioservice); 
  String buffer;
  pistream &is = p.get_stdout(); 

  async_read(os, &buffer, boost::bind(&end_read, placeholders::error)); 
  ioservice.run(); 
} 

void end_read(const boost::system::error_code &ec) 
{ 
} 

