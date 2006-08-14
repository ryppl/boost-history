//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <boost/coroutine/generator.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include<queue>

namespace coro = boost::coroutines;
using coro::generator;

typedef generator<void> job_type;

class scheduler {
public:
  void add(job_type job) {
    m_queue.push(job);
  }
  
  void reschedule(job_type::self& self) {
    add(current());
    self.yield();
  }

  job_type& current() {
    return m_queue.front();
  }

  void run () {
    while(!m_queue.empty()) {
      if(current()) {
	current()();
      }
      m_queue.pop();
    }
  }
private:

  std::queue<job_type> m_queue;
};

class message_queue {
public:
  std::string pop(job_type::self& self) {
    if(m_queue.empty()) {
      m_waiters.push(m_scheduler.current());
      self.yield();      
    }
    std::string res = m_queue.front();
    m_queue.pop();
    return res;
  }

  void push(const std::string& val) {
    m_queue.push(val);
    if(!m_waiters.empty()) {
      m_scheduler.add(m_waiters.front());
      m_waiters.pop();
    }
  }

  message_queue(scheduler& s) :
    m_scheduler(s) {}

private:
  std::queue<std::string> m_queue;
  std::queue<job_type> m_waiters;
  scheduler & m_scheduler;
};

scheduler global_scheduler;
message_queue mqueue(global_scheduler);

void producer(job_type::self& self, int id, int count) {
  while(count--) {
    std::cout << "In producer: "<<id<<"\n";
    mqueue.push("message from " + boost::lexical_cast<std::string>(id));
    std::cout << "\tmessage sent\n";
    global_scheduler.reschedule(self);
  } 
}

void consumer(job_type::self& self, int id) {
  while(true) {
    std::string result = mqueue.pop(self);
    std::cout <<"In consumer: "<<id<<"\n";
    std::cout <<"\tReceived: "<<result<<"\n";
    global_scheduler.reschedule(self);
  }
}

int main() {
  global_scheduler.run();
}
