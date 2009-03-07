//
// future.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace boost::join;

logger log1("log");

template <typename T>
class future : public actor {
public:
  typedef boost::function<T()> Computation;
  synch<T()> get;
private:
  Computation comp;
  async<void()> compute;
  async<void()> done;
  std::auto_ptr<T> value_;
  std::auto_ptr<std::runtime_error> except_;

public:
  future(Computation c, executor *e) : actor(e), comp(c) {
    chord(compute, &future::compute_body);
    chord(get, done, &future::get_body);
    compute();
  }
  void compute_body(async_o<void()> c) {
    try {
      value_.reset(new T(comp()));
    }
    catch (...) {
      //need more delicate way to catch application specific exceptions
      //such as Peter Dimov N2179 proposal
      //here for demo, simply change all exceptions to runtime_error
      except_.reset(new std::runtime_error("exceptions happens inside future computation"));
    }
    done();
  }
  T get_body(synch_o<T()> g, async_o<void()> d) {
    done(); //reissue done to allow multiple gets
    if(except_.get())
      throw *except_;
    return *value_;
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

std::string task1() {
  for(int i=0; i<8; i++) {
    log1.stream() << "task1 is running " << i << logger::endl;
    thread_sleep(1);
  }
  return std::string("task1 is done");
}

std::string task2() {
  for(int i=0; i<8; i++) {
    log1.stream() << "task2 is running " << i << logger::endl;
    thread_sleep(1);
    if(i==4) {
      throw std::runtime_error("future exception test");
    }
  }
  return std::string("task2 is done");
}

int main(int argc, char **argv) {
  executor exec(2);  //add 2 threads in executor thread pool
  future<std::string> fu1(task1, &exec.execute);
  future<std::string> fu2(task2, &exec.execute);
  for(int i=0; i<4; i++) {
    log1.stream() << "Main " << i << logger::endl;
    thread_sleep(1);
  }
  log1.msg("Main starts waiting on future");
  try {
    log1.stream() << "future1 returns : " << fu1.get() << logger::endl;
  }
  catch (...) {
    log1.msg("future1 got exception");
  }
  try {
    log1.stream() << "future2 returns : " << fu2.get() << logger::endl;
  }
  catch (...) {
    log1.msg("future2 got exception");
  }
  exec.shutdown();
  return 0;
}
