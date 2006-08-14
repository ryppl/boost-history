//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <boost/coroutine/generator.hpp>
#include <boost/bind.hpp>
#include<queue>

namespace coro = boost::coroutines;
using coro::generator;

typedef generator<void> job_type;

class scheduler {
public:
  void add(job_type job) {
    m_queue.push(job);
  }
  
  void reschedule() {
    add(current());
  }

  job_type& current() {
    return m_queue.front();
  }

  void run () {
    while(!m_queue.empty()) {
      if(current()) {
	current()();
	reschedule();
      }
      m_queue.pop();
    }
  }
private:

  std::queue<job_type> m_queue;
};

scheduler global_scheduler;

void printer(job_type::self& self, std::string name, int iterations) {
  while(iterations --) {
    std::cout<<name <<" is running, "<<iterations<<" iterations left\n";
    self.yield();
  }
  self.exit();
}

int main() {
  global_scheduler.add(boost::bind(printer, _1, "first", 10));
  global_scheduler.add(boost::bind(printer, _1, "second", 5));
  global_scheduler.add(boost::bind(printer, _1, "third", 3));
  global_scheduler.run();
}
