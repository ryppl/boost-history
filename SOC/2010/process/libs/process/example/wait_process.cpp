// access an unrelated process and wait synchronously 

#include <boost/process/all.hpp> 
#include <vector> 
#include <iterator> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  std::vector<process> processes; 
  create_snapshot(std::back_inserter(processes)); 
  process p = processes.front(); 
  status s = p.wait(); 
  if (s.exited()) 
    std::cout << s.exit_code() << std::endl; 
} 

