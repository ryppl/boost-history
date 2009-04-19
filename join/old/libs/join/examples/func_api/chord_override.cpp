//
// chord_override.cpp
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

using namespace boost::join;

logger log1("log");

//---- demo of "static" overriding by virtual chord body ----

//original buffer definitio
template <typename V>
class buffer: public actor {
public:
  async<void(V)> put;
  synch<V()> get;
  buffer() {
    chord(get, put, &buffer::chord_body);
  }
  //to be overriden, the chord body method has to be "virtual"
  virtual V chord_body(synch_o<V()> get, async_o<void(V)> put) {
    return put.arg1;
  }
};

//logged_buffer changes buffer's behaviour by overriding chord method
template <typename V>
class logged_buffer: public buffer<V> {
public:
  //override chord body method
  V chord_body(synch_o<V()> get, async_o<void(V)> put) {
    log1.stream() << "logged_buffer transfer: " << put.arg1 << logger::endl;
    return put.arg1;
  }
};

//---- demo of "dynamic" overiding by chord_override method ----

class transformer : public actor {
  buffer<std::string> &buf_;
public:
  async<void(std::string)> transform;
  async<void()> ready;
  transformer(buffer<std::string> &b, executor *e) : actor(e), buf_(b) {
    chord(transform, ready, &transformer::do_1);
    ready();
  }
  void do_1(async_o<void(std::string)> arg, async_o<void()> r) {
    buf_.put("do_1: " + arg.arg1);
    chord_override(transform, ready, &transformer::do_2);
    ready();
  }
  void do_2(async_o<void(std::string)> arg, async_o<void()> r) {
    buf_.put("do_2: " + arg.arg1);
    chord_override(transform, ready, &transformer::do_1);
    ready();
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
  logged_buffer<std::string> buf_;
  transformer trans_;
  async<void()> producer;
  async<void()> consumer;
  Demo(executor *e) : actor(e), trans_(buf_, e) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<5; i++) {
      ostr << i;
      trans_.transform(ostr.str());
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
  executor exec(3);  
  Demo demo(&exec.execute);
  demo.producer();
  demo.consumer();
  exec.shutdown();
  return 0;
}
