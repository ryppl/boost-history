// reads synchronously from parent process (suppose this program is always started as a child process) 

#include <boost/all/process.hpp> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  parent p; 
  pistream &is = p.get_stdout(); 
  std::cout << is.rdbuf(); 
} 

