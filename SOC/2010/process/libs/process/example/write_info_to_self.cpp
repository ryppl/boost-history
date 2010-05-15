// access child processes and write an attribute 

#include <boost/process/all.hpp> 
#include <iostream> 

using namespace boost::process; 

int main() 
{ 

  self &s = self::get_instance(); 
  s.set_priority(HIGH); //implement latter?

} 

