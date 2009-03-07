//
// buffer.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <string>
#include <iostream>
#include <sstream>

//using namespace boost;
using namespace boost::join;

logger log1("log");

std::string chord_body(synch_o<std::string(void)> get, async_o<void(std::string)> put) {
  return put.arg1;
}

class buffer: public actor {
public:
  async<void(std::string)> put;
  synch<std::string(void)> get;
  buffer() {
    chord(get, put, chord_body);
  }
};

void hello_world(buffer &b) {
  b.put("hello");
  b.put("world");
  log1.msg(b.get()+" ");
  log1.msg(b.get());
}

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class Demo : public actor {
public:
  buffer &buf_;
  async<void()> producer;
  async<void()> consumer;
  Demo(buffer &b, executor *e) : actor(e), buf_(b) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<5; i++) {
      ostr << i;
      buf_.put(ostr.str());
      log1.stream() << "producer sends [" << i << "]" << logger::endl;
      ostr.str("");
      thread_sleep(1);
    }
  }
  void consumer_cb(async_o<void()> c) {
    for(int i=0; i<5; i++) {
      log1.stream() << "consumer recvs [" << buf_.get() << "]" << logger::endl;      
      thread_sleep(2);
    }
  }
};

int main(int argc, char **argv) {
  buffer b;
  hello_world(b);

  executor exec(2);  //spawn 2 threads for executor thread pool
  Demo demo(b, &exec.execute);
  demo.producer();
  demo.consumer();
  exec.shutdown();
  return 0;
}
