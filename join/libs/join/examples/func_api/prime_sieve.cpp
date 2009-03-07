//
// prime_sieve.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace boost::join;

logger log1("log");

typedef rr_executor exec_type;
typedef actor_t<async<void(actor_base::callable)> > actor_type;

class prime_task : public actor_type {
  int my_prime_;
  int my_no_;
  prime_task * next_;
  exec_type *e_;
  //using async<> methods to represent states
  async<void()> init;
  async<void()> ready;
public:
  async<void(int)> sieve;
  void sieve_init_body(async_o<void(int)> value, async_o<void()> init) {
    my_prime_ = value;
    log1.stream() << "------ prime_task [" << my_no_ << "] found prime = " << my_prime_ << logger::endl;
    next_ = new prime_task(e_, my_no_+1); //create the next task
    ready();
  }
  void sieve_ready_body(async_o<void(int)> value, async_o<void()> ready) {
    int val = value;
    ready(); //allow processing the next incoming number as soon as possible
    if (val % my_prime_) { //not my multiples
      next_->sieve(val);
    } else { //my multiples
      log1.stream() << "prime_task [" << my_no_ << "] drop " << val << logger::endl;
    }
  }
  prime_task(exec_type *e, int no) : 
    actor_type(e->task_queue(no)), my_prime_(-1), my_no_(no), next_(0), e_(e) {
    chord(sieve, init, &prime_task::sieve_init_body);
    chord(sieve, ready, &prime_task::sieve_ready_body);
    init(); //initialize actor's state
  }
  ~prime_task() {
    if (next_ != 0) delete next_;
  }
};

int main(int argc, char **argv) {
  exec_type exec(4);

  int max_num;
  if (argc > 1)
    max_num = atoi(argv[1]); 
  else
    max_num = 1000;

  log1.stream() << "find primes in range [2-" << max_num << "]" << logger::endl;

  //create prime_task0 
  prime_task first_task(&exec,0);

  //generate integer series to be sieved and feed them to the chain of tasks
  for(int i=2; i<=max_num; i++)
    first_task.sieve(i);

  log1.msg("main thread shutdown...");
  exec.shutdown();
  return 0;
}
