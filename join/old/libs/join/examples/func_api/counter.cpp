//
// counter.cpp
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

class counter: public actor {
  async<void(long)> mystate;
public:
  async<void()> inc;
  synch<long()> value;

  void inc_bdy(async_o<void()> inc, async_o<void(long)> mi) {
    mystate(mi+1);
  }

  long val_bdy(synch_o<long()> val, async_o<void(long)> mi) {
    mystate(mi);
    return mi;
  }

  counter(executor *e) : actor(e) {
    chord(inc, mystate, &counter::inc_bdy);
    chord(value, mystate, &counter::val_bdy);
    mystate(0);
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
  counter c_;  
  async<void()> writer;
  async<void()> reader;
  Demo(executor *e) : actor(e), c_(e) {
    chord(writer, &Demo::writer_bdy);
    chord(reader, &Demo::reader_bdy);
  }
  void writer_bdy(async_o<void()> w) {
    for(int i=0; i<5; i++) {
      log1.msg("writer inc");
      c_.inc();
      thread_sleep(1);
    }
  }
  void reader_bdy(async_o<void()> r) {
    for(int i=0; i<5; i++) {
      log1.stream() << "reader reads [" << c_.value() << "]" << logger::endl;      
      thread_sleep(2);
    }
  } 
}; 

int main(int argc, char **argv) {
  executor exec(3);  //spawn 2 threads for executor thread pool
  Demo demo(&exec.execute);
  demo.reader();
  demo.writer();
  thread_sleep(8);
  log1.msg("---- executor exit ----");
  exec.shutdown(); 
  return 0;
}
