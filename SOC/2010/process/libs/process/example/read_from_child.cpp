// creates a child process and reads synchronously 

#include <boost/all/process.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  std::string exe = find_executable_in_path("hostname"); 
  child c = create_child(exe); 
  pistream &is = c.get_stdout(); 
  std::cout << is.rdbuf(); 
} 

