// write synchronously to parent process (suppose this program is always started as a child process) 

#include <boost/all/process.hpp> 

using namespace boost::process; 

int main() 
{ 
  parent p; 
  postream &os = p.get_stdin(); 
  os << "Hello, world!" << std::endl; 
} 

