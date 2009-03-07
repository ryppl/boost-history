//
// semaphore.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>

using namespace boost::join;

logger log1("log");

class semaphore : public actor {
public:
  async<void(void)> signal;
  synch<void(void)> wait;
  semaphore() {
    chord(wait, signal, &semaphore::wait_cb);
  }
  void wait_cb(synch_o<void(void)> wait, async_o<void(void)> signal) {
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class Demo : public actor {
public:
  async<void(int,semaphore&)> task;
  Demo(executor *e) : actor(e) {
    chord(task, &Demo::task_cb);
  }
  void task_cb(async_o<void(int,semaphore&)> task) {
    for(int i=0; i<10; i++) {
      log1.stream() << "Task " << task.arg1 << logger::endl;
      thread_sleep(task.arg1 * 1);
    }
    task.arg2.signal();
  }
};

int main(int argc, char **argv) {
  semaphore sema;
  executor exec(2);
  Demo demo(&exec.execute);
  demo.task(1,sema);
  demo.task(2,sema);
  for(int i=0; i<10; i++) {
    log1.stream() << "main..." << logger::endl;
    thread_sleep(1);
  }
  log1.stream() << "wait for tasks to finish" << logger::endl;
  sema.wait();
  sema.wait();
  log1.stream() << "both tasks finish" << logger::endl;
  exec.shutdown();
  return 0;
}

