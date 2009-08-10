//
// prime_sieve.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///* Copyright (c) 2005 Russ Cox, MIT; see COPYRIGHT */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/channel/channel.hpp>
#include <boost/channel/executors/thread_pool_executor.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

using namespace std;
using namespace boost::channel;

//define executor
typedef thread_pool_executor<mt_synch<boost_platform> > exec_type;

//define port type
typedef port<exec_type> port_type;
typedef port_type::choice_async choice_type;

static int task_no = 0;

//define task type: comprise of 3 parts:
//1> a port to recv msgs
//2> a run() method to process msgs
//3> a executor to execute run()
class prime_task {
private:
  //app data
  int my_prime_;
  int my_no_;
  prime_task *next_;

public:
 //task main data
  port_type port_;
  choice_type choice_;
  exec_type *exe_;

  prime_task(exec_type *e) :
    my_prime_(-1), next_(NULL),
    port_(e), choice_(e), exe_(e)
  {
    my_no_ = task_no++;
    choice_.bind(port_, boost::bind(&prime_task::run, this, _1));
  }
  ~prime_task() {
    if (next_ != NULL) delete next_;
    //cout << "prime_task [" << my_no_ << "] destroy " << endl;
  }
  void run(boost::shared_ptr<void> p)
  {
    int val = *(int *)p.get();
    if (my_prime_ == -1) { //first time
      my_prime_ = val;
      cout << "------ prime_task [" << my_no_ << "] found prime = " << val << endl;
      next_ = new prime_task(exe_); //create the next task
    }
    else {
      if (val % my_prime_) { //not my multiples
	next_->port_.send(p);
      } else { //my multiples
	cout << "prime_task [" << my_no_ << "] drop " << val << endl;
      }
    }
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

int main(int argc, char **argv) {
  //create executor with two threads
  exec_type exec(2);

  int max_num;
  if (argc > 1)
    max_num = atoi(argv[1]);
  else
    max_num = 1000;

  cout << "find primes in range [2-" << max_num << "]" << endl;

  //create prime_task0
  prime_task first_task(&exec);

  for(int i=2; i<=max_num; i++) {
    int *data = new int;
    *data = i;
    first_task.port_.send(data);
    //sleep 1 sec, to avoid jam executor queue & better show diff tasks running
    thread_sleep(1);
  }

  exec.wait(); //wait async tasks finish
  exec.shut_down_wait(); //wait threads exit

  return 0;

  //when main() exit, first_task goes out of scope, it will delete itself and all chained tasks
}
