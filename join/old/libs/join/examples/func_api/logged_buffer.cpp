//
// logged_buffer.cpp
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

//original buffer definition
template <typename V>
class buffer: public actor {
public:
  async<void(V)> put;
  synch<V()> get;
  buffer() {
    chord(get, put, &buffer::chord_body);
  }
  V chord_body(synch_o<V()> get, async_o<void(V)> put) {
    return put.arg1;
  }
};

//logged_buffer aggregates the original buffer and extends it with new functionality
template <typename V>
class logged_buffer: public actor {
  buffer<V> buf_;
public:
  //export inner actor's interface
  async<void(V)> &put;
  synch<V()> &get;
  //add new interface
  async<void(V)> logged_put;
  logged_buffer(executor *e) : actor(e), buf_(), put(buf_.put), get(buf_.get) {
    chord(logged_put, &logged_buffer::chord_body);
  }
  void chord_body(async_o<void(V)> put) {
    log1.stream() << "logged_put: " << put.arg1 << logger::endl;
    buf_.put(put.arg1);
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
  async<void()> producer;
  async<void()> consumer;
  Demo(executor *e) : actor(e), buf_(e) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<5; i++) {
      ostr << i;
      if (i%2 == 0)
        buf_.put(ostr.str());
      else
        buf_.logged_put(ostr.str());
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
