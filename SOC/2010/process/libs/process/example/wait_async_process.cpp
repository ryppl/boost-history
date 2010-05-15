// access an unrelated process and wait asynchronously 

#include <boost/process/all.hpp> 
#include <boost/asio.hpp> 
#include <boost/bind.hpp> 
#include <vector> 
#include <iterator> 
#include <iostream> 

using namespace boost::process; 
using namespace boost::asio; 

io_service ioservice; 
status s(ioservice); 

void end_wait(const boost::system::error_code &ec); 

int main() 
{ 
  std::vector<process> processes; 
  create_snapshot(std::back_inserter(processes)); 
  process p = processes.front(); 
  // if status is an I/O object it must be bound to an I/O service object; 
  // we pass ioservice as a parameter to bind the status object which is 
  // created within the status() method to the I/O service object; 
  status &s = p.status(ioservice); 
  s.async_wait(boost::bind(&end_wait, placeholders::error)); 
  ioservice.run(); 
} 

void end_wait(const boost::system::error_code &ec) 
{ 
  if (!ec) 
    std::cout << "process exited" << std::endl; 
} 

