// access unrelated processes and read an attribute 

#include <boost/process/all.hpp> 
#include <boost/foreach.hpp> 
#include <vector> 
#include <iterator> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 
  std::vector<process> processes; 

  options opts; 
  opts.children_only = true; 
  // without any option the snapshot contains all processes; 
  // if and how many options Boost.Process is going to support 
  // depends on how much time there is to implement them 
  create_snapshot(std::back_inserter(processes), opts); 

  BOOST_FOREACH(process &p, processes) 
    std::cout << p.name() << std::endl; 
} 

