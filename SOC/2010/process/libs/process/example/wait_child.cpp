// creates a child process and waits synchronously 

#include <boost/process/all.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  std::string exe = find_executable_in_path("hostname"); 
  child c = create_child(exe); 
  status s = c.wait(); 
  if (s.exited()) 
    std::cout << s.exit_code() << std::endl; 
} 

