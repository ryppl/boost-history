// creates a child process using the high-level API 

#include <boost/process/all.hpp> 
#include <boost/assign/list_of.hpp> 
#include <string> 
#include <utility> 

using namespace boost::process; 

int main() 
{ 
  process_factory f("C:\\Windows\\notepad.exe"); 
  f.environment.insert(std::make_pair("NEW_ENV_VARIABLE", "VALUE")); 
  child = f.create(); 
} 

