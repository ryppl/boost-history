// creates a child process and reads synchronously 

#include <boost/all/process.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  std::string exe = find_executable_in_path("hostname"); 
  context ctx; 
  // by default streams are inherited; if this process wants to read 
  // from child's stdout the stream must be redirected (= captured) 
  ctx.stdout_behavior = capture; 
  child c = create_child(exe, ctx); 
  pistream &is = c.get_stdout(); 
  std::cout << is.rdbuf(); 
} 

