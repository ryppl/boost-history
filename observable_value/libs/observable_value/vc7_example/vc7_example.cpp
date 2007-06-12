#include <iostream>
#include <tchar.h>

#include "../boost/observable_value.hpp"


void bar(const int& value) { std::cout << "Changed: " << value << std::endl; }
void foo(const boost::observable_value<int>& value) 
{ 
  std::cout << "Original: " << value << std::endl; 
  value.connect(&bar);
}

int _tmain(int argc, _TCHAR* argv[])
{
  boost::observable_value<int> i(1);
  foo(i);
  i=1;  // no change
  i=0;  // change
  i+=0; // no change
  i+=5; // change
	return 0;
}

