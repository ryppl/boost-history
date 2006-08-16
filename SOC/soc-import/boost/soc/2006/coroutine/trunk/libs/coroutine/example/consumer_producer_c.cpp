//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>

// peers version of the producer/consumer pattern.
namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<void(const std::string&)> consumer_type;
typedef coroutine<void()> producer_type;

void producer_body(producer_type::self& self, 
		   std::string base, 
		   consumer_type& consumer) {
  std::sort(base.begin(), base.end());
  do {
    self.yield_to(consumer, base);
  } while (std::next_permutation(base.begin(), base.end()));
}

void consumer_body(consumer_type::self& self, 
		   const std::string& value,
		   producer_type& producer) {
  std::cout << value << "\n";
  while(true) {
    std::cout << self.yield_to(producer)<< "\n";
  } 
}

int main() {
  {
    producer_type producer;
    consumer_type consumer;
    
    producer = producer_type
      (boost::bind
       (producer_body, 
	_1, 
	"hello", 
	boost::ref(consumer)));

    consumer = consumer_type
      (boost::bind
       (consumer_body, 
	_1,
	_2,
	boost::ref(producer)));
       
    producer();
  }

  {
    producer_type producer;
    consumer_type consumer;
    
    consumer = consumer_type
      (boost::bind
       (consumer_body, 
	_1,
	_2,
	boost::ref(producer)));
       
    producer = producer_type
      (boost::bind
       (producer_body, 
	_1, 
	"hello", 
	boost::ref(consumer)));
       
    consumer(std::string());
  }  
}
