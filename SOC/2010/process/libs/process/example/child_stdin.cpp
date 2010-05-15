// creates a child process and writes synchronously 

#include <boost/all/process.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  std::string exe = find_executable_in_path("ftp"); 
  context ctx; 
  // by default streams are inherited; if this process wants to write 
  // to child's stdin the stream must be redirected (= captured) 
  ctx.stdin_behavior = capture; 
  child c = create_child(exe, ctx); 
  postream &os = c.get_stdin(); 
  os << "quit" << std::endl; 
} 

