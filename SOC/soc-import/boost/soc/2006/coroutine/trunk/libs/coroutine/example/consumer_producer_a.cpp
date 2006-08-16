//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <boost/coroutine/generator.hpp>

// Consumer driven producer/consumer pattern.
namespace coroutines = boost::coroutines;
using coroutines::generator;

typedef generator<const std::string&> generator_type;

const std::string& producer(generator_type::self& self, std::string base) {
  std::sort(base.begin(), base.end());
  do {
    self.yield(base);
  } while (std::next_permutation(base.begin(), base.end()));
  self.exit();
}

template<typename Producer>
void consumer(Producer producer) {
  do {
    std::cout <<*producer << "\n";
  } while(++producer);
}

struct filter {
  typedef const std::string& result_type;

  template<typename Producer>
  const std::string& operator()
    (generator_type::self& self, Producer producer) {
    do {
      self.yield(*producer + " world");
    } while(++producer);

    self.exit();
    // gcc complains here while it doesn't for `producer`... werid.
    // the abort quiets it.
    abort();
    // it doesn't quiet VCPP8 though!
  }
};
int main() {
  consumer
    (generator_type
     (boost::bind
      (producer, _1, std::string("hello"))));

  
  consumer
    (generator_type
     (boost::bind
      (filter(),
       _1,
       generator_type
       (boost::bind
	(producer, _1, std::string("hello"))))));
  
}
