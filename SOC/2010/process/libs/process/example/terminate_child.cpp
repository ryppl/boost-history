// creates a child and terminates it 

#include <boost/all/process.hpp> 
#include <string> 

using namespace boost::process; 

int main() 
{ 
  std::string exe = find_executable_in_path("hostname"); 
  child c = create_child(exe); 
  c.terminate(); 
} 


