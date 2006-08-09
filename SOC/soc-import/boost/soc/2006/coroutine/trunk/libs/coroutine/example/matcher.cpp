//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <boost/coroutine/coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

bool matcher(coroutine<bool(char)>::self& self, char c) { 
  while(true) { 
    if(c == 'h') { 
      c = self.yield(false); 
      if(c == 'e') { 
	c = self.yield(false); 
	if(c == 'l') { 
	  c = self.yield(false); 
	  if(c == 'l') { 
	    c = self.yield(false); 
	    if(c == 'o') { 
	      c = self.yield(true); 
	    } continue; 
	  } else continue; 
	} else continue; 
      } else continue; 
    } c = self.yield(false); 
  } 
} 

int main(int, char**) { 
  coroutine<bool(char)> match(matcher); 
  std::string buffer = "hello to everybody, this is not an hello world program.";
  for(std::string::iterator i = buffer.begin(); 
      i != buffer.end(); 
      ++i) { 
    if(match(*i)) std::cout<< "match\n"; 
  } 
} 
 
