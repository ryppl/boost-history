//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <boost/coroutine/coroutine.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>
// This is a direct port of the pattern matching algorithm, 
// written in Lua, found in [Moura04b].
//
// The idea behind the algorithm is to use a goal driven desing:
//
//
// It is slightly ineficient because it uses
// boost::bind and the lack of decltype for automatic deduction of the return
// type requires the use of boost::function to wrap the result
// function object of prim, seq and alt in a known runtime-polymorphic type.
//
// While using coroutines for pattern matching is not the most
// efficient solution, the elegance of this design can probably be matched 
// only by an implementation using full continuations.
//

namespace coro = boost::coroutines;
using coro::coroutine;

typedef coroutine<std::size_t(const std::string&, std::size_t)> 
coroutine_type;

// used in lieu of decltype.
typedef boost::function<std::size_t(coroutine_type::self&, const std::string& , std::size_t)> function_type;

std::size_t prim_impl(coroutine_type::self& self, 
		      const std::string& str,
		      const std::string& input,
		      std::size_t pos) {
  if(input.substr(pos, str.length()) == str) {
     self.yield(pos + str.length());
  }
  return std::string::npos;
}

// returns a function object that matches a string.
function_type prim(const std::string& str) {
  return boost::bind(prim_impl, _1, str, _2, _3);
}

std::size_t alt_impl(coroutine_type::self& self,
		     const function_type& pattern1,
		     const function_type& pattern2,
		     const std::string& input,
		     const std::size_t pos) {
  pattern1(self, input, pos);
  pattern2(self, input, pos);
  return std::string::npos;
}

// returns a function object that matches one of two patterns.
function_type alt(const function_type& pattern1,
		  const function_type& pattern2) {
  return boost::bind(alt_impl, _1, pattern1, pattern2, _2, _3);
}

std::size_t seq_impl(coroutine_type::self& self,
		     const function_type& pattern1,
		     const function_type& pattern2,
		     const std::string& input,
		     const std::size_t pos) {
  coroutine_type coro (pattern1);
  while(coro) {
    std::size_t val = coro(input, pos);
    if(val != std::string::npos) 
      pattern2(self, input, val);
    else break;
  }
  return std::string::npos;
}
		     
// retuns a function object that matches the conjuction of two patterns.
function_type seq(const function_type& pattern1,
		  const function_type& pattern2) {
  return bind(seq_impl, _1, pattern1, pattern2, _2, _3);
}

bool match(const std::string& str, const function_type& pattern) {
  coroutine_type coro(pattern);
  while(coro) {
    std::size_t pos = coro(str , 0);
    if(pos == std::string::npos) break;
    if(pos == str.length()) 
      return true;
  }
  return false;
}

int main() {
  function_type pattern1 (prim("hello"));
  function_type pattern2 (seq(prim("hello"), prim(" world")));
  function_type pattern3 (seq(prim("hello"), alt(prim(" world"), prim(" universe"))));
  std::cout <<"first match:" <<  match("hello", pattern1) <<std::endl;
  std::cout <<"second match: "<< match("hello world", pattern2) <<std::endl;
  std::cout <<"third match: "<< match("not hello _world", pattern2) <<"\n"; // expected fail
  std::cout <<"fourth match: "<< match("hello world", pattern3) <<std::endl;
  std::cout <<"fifth match: "<< match("hello universe", pattern3) <<std::endl;
}
