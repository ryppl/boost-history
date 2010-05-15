// creates a child process and writes asynchronously 

#include <boost/all/process.hpp> 
#include <boost/asio.hpp> 
#include <boost/bind.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 

void end_write(const boost::system::error_code &ec); 

int main() 
{ 
  std::string exe = find_executable_in_path("ftp"); 
  context ctx; 
  // if we want to use asynchronous I/O we must pass a pointer to an 
  // I/O service object; the I/O service object is used to initialize 
  // I/O objects of type pistream and postream within the child object 
  context.io_service = &ioservice; 
  // by default streams are inherited; if this process wants to write 
  // to child's stdin the stream must be redirected (= captured) 
  ctx.stdin_behavior = capture; 
  child c = create_child(exe, ctx); 
  postream &os = c.get_stdin(); 
  async_write(os, buffer("quit\n"), boost::bind(&end_write, placeholders::error)); 
  ioservice.run(); 
} 

void end_write(const boost::system::error_code &ec) 
{ 
} 

