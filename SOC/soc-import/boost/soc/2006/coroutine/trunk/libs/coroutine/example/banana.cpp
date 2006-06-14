//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

template<typename Iter>
bool match(Iter first, Iter last, std::string match) {
  std::string::iterator i = match.begin();
  i != match.end();
  for(; (first != last) && (i != match.end()); ++i) {
    if (*first != *i)
      return false;
    ++first;
  }
  return i == match.end();
}

template<typename BidirectionalIterator> 
BidirectionalIterator 
match_substring(BidirectionalIterator begin, 
		BidirectionalIterator end, 
		std::string xmatch,
		coroutine<BidirectionalIterator(void)>& self) { 
  BidirectionalIterator begin_ = begin;
  for(; begin != end; ++begin) 
    if(match(begin, end, xmatch)) {
      self.yield(begin);
    }
  return end;
} 

typedef coroutine<std::string::iterator(void)> coroutine_type;
int main(int, char**) { 
  std::string buffer = "banananana"; 
  std::string match = "nana"; 
  std::string::iterator begin = buffer.begin();
  std::string::iterator end = buffer.end();

  typedef std::string::iterator signature(std::string::iterator, 
					  std::string::iterator, 
					  std::string,
					  coroutine_type&);

  coroutine<std::string::iterator(void)> matcher
      (boost::bind(static_cast<signature*>(match_substring), 
		   begin, 
		   end, 
		   match, 
		   _1)); 

  std::string::iterator i = matcher();
  while(matcher && i != buffer.end()) {
    std::cout <<"Match at: "<< std::distance(buffer.begin(), i)<<'\n'; 
    i = matcher();
  }
} 
