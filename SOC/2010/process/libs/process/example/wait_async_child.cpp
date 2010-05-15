// creates a child process and reads asynchronously 

#include <boost/all/process.hpp> 
#include <boost/asio.hpp> 
#include <boost/array.hpp> 
#include <boost/bind.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 
pistream *is; 
boost::array<char, 4096> buf; 

void begin_read(); 
void end_read(const boost::system::error_code &ec, std::size_t bytes_transferred); 

int main() 
{ 
  std::string exe = find_executable_in_path("hostname"); 
  context ctx; 
  ctx.io_service = &ioservice; 
  child c = create_child(exe, ctx); 
  is = &c.get_stdout(); 
  begin_read(); 
  ioservice.run(); 
} 

void begin_read() 
{ 
  is->async_read_some(buffer(buf), boost::bind(&end_read, placeholders::error, placeholders::bytes_transferred)); 
} 

void end_read(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{ 
  if (!ec) 
  { 
    std::cout << std::string(buf.data(), bytes_transferred) << std::flush; 
    begin_read(); 
  } 
} 


